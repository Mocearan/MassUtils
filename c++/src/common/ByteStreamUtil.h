
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/utils/ByteStreamUtil.h
* -> Author       : Mass
* -> Date         : 2020-08-14 17:38:29
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-08-14 17:38:30
* -> Description  : 
            Bytes stream Implement
* =============================================================================*/

#ifndef __MASSUTILS_COMMON_BYTESTREAMUTIL_H__
#define __MASSUTILS_COMMON_BYTESTREAMUTIL_H__


#include <memory>


BEGIN_NAMESPACE_MASS

#ifdef WIN32
    #ifdef BUILD_PDU
        #define DLL_MODIFIER __declspec(dllexport)
    #else
        #define DLL_MODIFIER __declspec(dllimport)
    #endif
#else
    #define DLL_MODIFIER
#endif

enum class ByteStreamErrCode : char
{
    ERROR_PARSE_FAILED = 1;
//        ERROR_WRONG_SERVICE_ID;
//        ERROR_WRONG_COMMAND_ID;
//        ERROR_ALLOC_FAIED;
};


class CByteStreamException
{
public:
    CByteStreamException(uint32_t serviceId, uint32_t commandId, uint32_t errorCode, const std::string &errorMsg)
        : m_service_id{ServiceID}, m_command_id{commandId}, m_error_code{errorCode}, m_error_msg{errorMsg}
    {}

    CByteStreamException(uint32_t errorCode, const std::string &errorMsg)
        : m_service_id{0}, m_command_id{0}, m_error_code{errorCode}, m_error_msg{errorMsg}
    {}

    virtual ~CByteStreamException() {}

    uint32_t GetServiceID() const { return m_service_id; }
    uint32_t GetCommandID() const { return m_command_id; }
    uint32_t GetErrCode() const { return m_error_code; }
    const std::string &GetErrMsg() const { return m_error_msg; }

private:
    uint32_t m_service_id;
    uint32_t m_command_id;
    uint32_t m_error_code;
    std::string m_error_msg;
};


class DLL_MODIFIER CSimpleBuffer
{
    // a simple buffer class that supports capacity expansion.
    /*
    *       buffer: |dsfasfasfafasa|                |
    *               |m_write_offset|
    *               |         m_alloc_size          | 
    *            m_buffer    
    */
public:
    CSimpleBuffer();
    ~CSimpleBuffer();

    uchar_t *GetBuffer() { return m_buffer; }
    uint32_t GetAllocSize() const { return m_alloc_size; }
    uint32_t GetWriteOffset() const { return m_write_offset; }
    void Incwriteoffset(uint32_t len) { m_write_offset += len; }


    /*
    * -> Description:   increase buffer size.
    * -> param {type} len : the size to expand
    */
    void Extend(uint32_t len);


    /* 
    * -> Description:   write data to the buffer.
    * -> param {type}   
    *         buf :     the data to write
    *         len :     the length of data to be written
    * -> return {type}  the actual length of the data being written
    */
    uint32_t Write(const void *buf, uint32_t len);


    /* 
    * -> Description: read data from the buffer
    * -> param {type} 
    *       buf :     the data to read out
    *       len :     the length of data to be read
    * -> return {type} : the actual length of the data being read
    */
    uint32_t Read(void *buf, uint32_t len);

private:
    
    /* 
    * -> Description: the buffer data container.
    * -> type :    uchar_t == unsigned char
    */
    uchar_t* m_buffer = nullptr;
    
    /* 
    * -> Description: the buffer allocated size
    */
    uint32_t m_alloc_size = 0;
    
    /* 
    * -> Description: actual data ending.
    */              
    uint32_t m_write_offset = 0;
};


class ByteStream
{
    /*
        Buffer support 2 implementation:
            1. n raw uchar_t stream, ref from outside, capacity 
                expansion is not supported.
            2. a buffer object implemented as CSimpleBuffer, ref
                from outside, support expand capacity
    */
public:
    
    /* 
    * -> Description: 
    *           implement by raw uchar_t stream
    */
    ByteStream(uchar_t *buf, uint32_t len);
    
    /* 
    * -> Description: 
    *           implement by CSimpleBuffer stream
    */
    ByteStream(std::shared_ptr<CSimpleBuffer> &pSimpleBuf, uint32_t pos);

    ~ByteStream() {}

public:
    
    /* 
    * -> Description: 
    *       get the buffer handle as raw uchar_t stream
    * -> return {uchar_t}  
    */
    uchar_t *GerBuf() { return m_pSimpBuf ? m_pSimpBuf->getBuffer() : m_pBuf; }

    /* 
    * -> Description: get data length     
    */
    uint32_t GetPos() { return m_pos; }

    /* 
    * -> Description: get buffer length
    */
    uint32_t GetLen() { return m_len; }

    /* 
    * -> Description: skip a few bytes
    */
    void Skip(uint32_t len);

    /* 
    * -> Description: read a 16 bit int from bytes stream
    */
    static int16_t ReadInt16(uchar_t *buf);

    /* 
    * -> Description:  read a 16 bit uint from bytes stream
    */
    static uint16_t ReadUint16(uchar_t *buf);

    /*
    * -> Description: read a 32bit int from bytes stream
    */
    static int32_t ReadInt32(uchar_t *buf);

    /* 
    * -> Description: read a 32bit uint from bytes streem
    */
    static uint32_t ReadUint32(uchar_t *buf);

    /* 
     * -> Description: write a 16bit int to bytes stream
     * -> param {type} 
     *          [out] buf
    */
    static void WriteInt16(uchar_t *buf, int16_t data)

    /* 
     * -> Description: write a 32bit int to bytes stream
     * -> param {type} 
     *          [out] buf
    */
    static void WriteInt32(uchar_t *buf, int16_t data);
    
    /* 
     * -> Description: write a 16bit uint to bytes stream
     * -> param {type} 
     *          [out] buf
    */
    static void WriteUint16(uchar_t *buf, int16_t data);
    
    /* 
     * -> Description: write a 32bit uint to bytes stream
     * -> param {type} 
     *          [out] buf
    */
    static void WriteUint32(uchar_t *buf, int16_t data);

    /* 
    * -> Description: write a string to bytes stream
    * -> param {type} 
    * */
    void WriteStr(const std::string &str);

    [[deprecated]] 
    void WriteStr(const char* str, uint32_t len);
    
    /* 
    * -> Description: read a string from bytes stream
    * -> param {type} :
    *       [out] len: string length
    * -> return {type} : the read string
    */
    char* ReadStr(uint32_t& len);

    /* 
    * -> Description: write a packet to bytes stream 
    * -> param {type} 
    *       [in] : data
    *       [in] len : data length
    */
    void WriteData(const uchar_t* data, uint32_t len);

    /* 
    * -> Description: read a packet from bytes stream 
    * -> param {type} 
    *       [in] : data
    *       [in] len : data length
    */
    uchar_t* ReadData( uint32_t &len);

public:
    /// stream operator reload

    /* 
    * -> Description:  write a 8bit int to bytes stream
    * */
    void operator<<(int8_t data);

    /* 
    * -> Description:  write a 8bit uint to bytes stream
    * */
    void operator<<(uint8_t data);

    /* 
    * -> Description:  write a 16bit int to bytes stream
    * */
    void operator<<(int16_t data);

    /* 
    * -> Description:  write a 16bit uint to bytes stream
    * */
    void operator<<(uint16_t data);

    /* 
    * -> Description:  write a 32bit int to bytes stream
    * */
    void operator<<(int32_t data);

    /* 
    * -> Description:  write a 32bit uint to bytes stream
    * */
    void operator<<(uint32_t data);

    /* 
    * -> Description:  read a 8bit int from bytes stream
    * */
    void operator>>(int8_t& data);

    /* 
    * -> Description:  read a 8bit uint from bytes stream
    * */
    void operator>>(uint8_t& data);

    /* 
    * -> Description:  read a 16bit int from bytes stream
    * */
    void operator>>(int16_t& data);

    /* 
    * -> Description:  read a 16bit uint from bytes stream
    * */
    void operator>>(uint16_t& data);

    /* 
    * -> Description:  read a 32bit int from bytes stream
    * */
    void operator>>(int32_t& data);

    /* 
    * -> Description:  read a 32bit uint from bytes stream
    * */
    void operator>>(uint32_t& data);

private:

    /*     
    * -> Description: 
    *          a common write func. 
    *          used to encapsulate both buffer implementations 
    * -> param {type} :
    *           [in] buf
    */
    void _write_byte(const void *buf, uint32_t len);
    
    /*     
    * -> Description: 
    *           a common read func.
    *           used to encapsulate both buffer implementations
    * -> param {type} :
    *           [out] buf
    */
    void _read_byte(void *buf, uint32_t len);

private:

   /* 
    * -> Description: implement by CSimpleBuffer object
    */
   std::shared_ptr<CSimpleBuffer>
       m_pSimpBuf;

   /* 
    * -> Description: implement by raw uchar_t stream
    */
   uchar_t *m_pBuf;

   /* 
    * -> Description: actual data ending.
    */
   uint32_t m_pos;

   /* 
    * -> Description: the buffer length
    */
   uint32_t m_len;
};


END_NAMESPACE_MASS

#endif //__MASSUTILS_CPP_COMMON_BYTESTREAMUTIL_H__