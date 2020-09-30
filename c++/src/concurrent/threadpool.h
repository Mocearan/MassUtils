
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/utils/threadpool.h
* -> Author       : Mass
* -> Date         : 2020-07-01 13:55:43
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-07-01 13:59:10
* -> Description  : 
* =============================================================================*/

#ifndef __MASSUTILS_CONCURRENT_THREADPOOL_H__
#define __MASSUTILS_CONCURRENT_THREADPOOL_H__

#include "basedef.h"

#include <condition_variable>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#ifndef MASSUTILS_THREADPOOL_COUNT
#define MASSUTILS_THREADPOOL_COUNT 4
#endif

BEGIN_NAMESPACE_MASS

interface CMTaskQueue {
public:
    // CMTaskQueue() = default;
    // virtual ~CMTaskQueue() = default;

    virtual void enqueue(MTask task) = 0;
    //virtual size_t Size() = 0;
    virtual void shutdown() = 0;
};

#if MASSUTILS_THREADPOOL_COUNT > 0
class CMThreadPool final
    : noncopyable,
      public CMTaskQueue
{
    struct worker
    {
        // ref avoid loop reference.
        CMThreadPool &pool_;

        explicit worker(ThreadPool &pool) pool_{pool} {}

        void operator()()
        {
            for (;;)
            {
                MTask task;
                {
                    std::unique_lock<std::mutex> lock(pool_.mutex_);

                    pool_.cond_.wait(lock, [&]() {
                        return not pool_.jobs_list.empty() or pool_.shutdown;
                    });

                    if (pool_.shutdowned and pool_.jobs_list.empty())
                    {
                        break;
                    }

                    tasks = pool_.jobs_list.front();
                    pool_.jobs_list.pop_front();
                }

                assert(true == static_cast(bool)(tasks));
                tasks();
            }
        }
    };

    friend struct worker;

public:
    explicit CMThreadPool(size_t n)
    {
        for (; n; thread_list.emplace_back(worker(*this)), --n)
            ;
    }

    ~CMThreadPool() override = default;

    void enqueue(MTask task) override
    {
        std::unique_lock<std::mutex> lock(mutex);
        jobs_list.push_back(task);
        cond_.notify_one();
    }

    void shutdown() override
    {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            shutdowned = true;
        }

        cond_.notify_all();

        //join..
        for (auto &t : thread_list)
            t.join();
    }

private:
    std::vector<std::thread> thread_list;
    std::list<MTask> jobs_list;

    bool shutdowned{false};

    std::condition_variable cond_;
    std::mutex mutex_;
};
#else // single thread
class CMSingleThread final
    : public TaskQueue
{
public:
    CMSingleThread() = default;
    ~CMSingleThread() = default;

    void enqueue(MTask tasks) override
    {
        std::thread([=]() {
            {
                std::lock_guard<std::mutex> lock(running_threads_mtx);
                ++running_threads;
            }

            tasks();

            {
                std::lock_guard<std::mutex> lock(running_threads_mtx);
                --running_threads;
            }
        }).detach();
    }

    virtual void shutdown() override
    {
        for (;;)
        {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            std::lock_guard<std::mutex> lock(running_threads_mtx);
            if (not running_threads)
                break;
        }
    }

private:
    std::mutex running_threads_mtx{};
    int running_threads = 0;
};
#endif

END_NAMESPACE_MASS

#endif //__MASSUTILS_CONCURRENT_THREADPOOL_H__