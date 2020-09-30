
/* =============================================================================
* -> FilePath     : /WeTalkServer2/src/base/common/ByteStreamUtil.cpp
* -> Author       : Mass
* -> Date         : 2020-08-14 17:37:36
* -> version      : 
* -> LastEditors  : Mass
* -> LastEditTime : 2020-08-17 10:38:13
* -> Description  : 
* =============================================================================*/

#include "ByteStreamUtil.h"

#include <cstdlib> // for realloc
#include <cstring> // for mem*  

BEGIN_NAMESPACE_MASS

///   CSimpleBuffer  Definition

CSimpleBuffer::CSimpleBuffer() 
{
}

CSimpleBuffer::~CSimpleBuffer()
{
    m_alloc_size = 0;
    m_write_offset = 0;
    if(m_buffer)
        free(m_buffer);
}

void CSimpleBuffer::Extend(uint32_t len) 
{
    // since there is not enough allocated space in buffer,
    //  m_alloc_size make no sense until it recalculates the size to 
    // be allocated. So first change m_alloc_size to the size that
    // the actual data is going to grow to.
    m_alloc_size = m_write_offset + len;
    // Has the actual data size redundancy 1/4 for the new m_alloc_size
    m_alloc_size += m_alloc_size >> 2;

    m_buffer = static_cast<uchar_t *>(std::realloc(m_buffer, m_alloc_size));
}


uint32_t CSimpleBuffer::Write(const void *buf, uint32_t len) 
{
    if(m_write_offset + len > m_alloc_size)
        Extend(len);

    if(buf) // write expand data to the end of actual data in buffer
        std::memcpy(m_buffer + m_write_offset, buf, len);

    m_write_offset += len;

    return len;
}


uint32_t CSimpleBuffer::Read(void *buf, uint32_t len) 
{
    if(len > m_write_offset)
        len = m_write_offset;

    if(buf) // read data from the begin of actual data in buffer
        std::memcpy(buf, m_buffer, len);

    m_write_offset -= len;
    // remove the data been read. 
    // From the end of the data already read, move the remaining 
    // m_write_offset data to the beginning of the buffer
    std::memmove(m_buffer, m_buffer + len, m_write_offset);
    return len;
}


///  ByteStream  Definition

ByteStream::ByteStream(uchar_t *buf, uint32_t len)
    : m_pBuf{buf}, m_len{len}, m_pSimpBuf{nullptr}, m_pos{0}
{
}

ByteStream::ByteStream(std::shared_ptr<CSimpleBuffer> &pSimpleBuf, uint32_t pos)
    : m_pSimpBuf{pSimpleBuf}, m_pos{pos}, m_pBuf{nullptr}, m_len{0}
{
}


void ByteStream::Skip(uint32_t len) 
{
    m_pos += len;
    if(m_pos > m_len)
        throw CByteStreamException(ByteStreamCode::ERROR_PARSE_FAILED, "parse packet failed!");
}

int16_t ByteStream::ReadInt16(uchar_t *buf) 
{
    return static_cast<int16_t>((buf[0] << 8) bitor buf[1]);
}

uint16_t ByteStream::ReadUint16(uchar_t* buf)
{
    return static_cast<uint16_t>((buf[0] << 8) bitor buf[1]);
}


int32_t ByteStream::eadInt32(uchar_t* buf)
{
    return static_cast<int32_t>((buf[0] << 24) bitor 
                    (buf[1] << 16) bitor
                    (buf[2] << 8)  bitor
                    buf[3]);
}


uint32_t ByteStream::ReadUint32(uchar_t* buf)
{
    return static_cast<uint32_t>((buf[0] << 24) bitor 
                    (buf[1] << 16) bitor
                    (buf[2] << 8)  bitor
                    buf[3]);
}

void ByteStream::WriteInt16(uchar_t *buf, int16_t data)
{
    buf[0] = static_cast<uchar_t>(data >> 8);
    buf[1] = static_cast<uchar_t>(data bitand 0xFF);
}

void ByteStream::WriteInt32(uchar_t *buf, int16_t data)
{
    buf[0] = static_cast<uchar_t>(data >> 24);
    buf[1] = static_cast<uchar_t>((data >> 16) bitand 0xFF);
    buf[2] = static_cast<uchar_t>((data >> 8) bitand 0xFF);
    buf[3] = static_cast<uchar_t>(data bitand 0xFF);
}

void ByteStream::WriteUint16(uchar_t *buf, int16_t data)
{
    buf[0] = static_cast<uchar_t>(data >> 8);
    buf[1] = static_cast<uchar_t>(data bitand 0xFF);
}

void ByteStream::WriteUint32(uchar_t *buf, int16_t data)
{
    buf[0] = static_cast<uchar_t>(data >> 24);
    buf[1] = static_cast<uchar_t>((data >> 16) bitand 0xFF);
    buf[2] = static_cast<uchar_t>((data >> 8) bitand 0xFF);
    buf[3] = static_cast<uchar_t>(data bitand 0xFF);
}

void ByteStream::_write_byte(const void* buf, uint32_t len)
{
    // avoid buffer overflow.
    // CSimpleBuffer implement support expand capacity, so it's safe
    // hence, need to check that the raw buffer exists and that 
    //  the write does not overflow
    if (m_pBuf and (m_pos + len > m_len))
        return;
    
    // select buffer according to the implementation.
    if(m_pSimpBuf)
        m_pSimpBuf->Write(/*(char *)*/buf, len);
    else
        memcpy(m_pBuf + m_pos, buf, len);

    m_pos += len;
}

void ByteStream::_read_byte(void* buf, uint32_t len)
{
    if(m_pos + len > m_len)
        throw CByteStreamException(ByteStreamErrCode::ERROR_PARSE_FAILED, "parse packet failed!");

    if (m_pSimBuf)
        m_pSimBuf->Read(/*(char *)*/buf, len);
    else
        memcpy(buf, m_pBuf + m_pos, len);

    m_pos += len;
}

void ByteStream::operator<<(int8_t data) 
{
    _write_byte(&data, 1);
}

void ByteStream::operator<<(uint8_t data) 
{
    _write_byte(&data, 1);
}

void ByteStream::operator<<(int16_t data)
{
    uchar_t buf[2] {
        static_cast<uchar_t>(data >> 8),
        static_cast<uchar_t>(data bitand 0xFF)
    };

    _write_byte(buf, 2);
}


void ByteStream::operator<<(uint16_t data)
{
    uchar_t buf[2] {
        static_cast<uchar_t>(data >> 8),
        static_cast<uchar_t>(data bitand 0xFF)
    };

    _write_byte(buf, 2);
}

void ByteStream::operator<<(int32_t data)
{
    uchar_t buf[4]{
        static_cast<uchar_t>(data >> 24),
        static_cast<uchar_t>((data >> 16) bitand 0xFF),
        static_cast<uchar_t>((data >> 8) bitand 0xFF),
        static_cast<uchar_t>(data bitand 0xFF)
    };
    
    _write_byte(buf, 4); 
}

void ByteStream::operator<<(uint32_t data)
{
    uchar_t buf[4]{
        static_cast<uchar_t>(data >> 24),
        static_cast<uchar_t>((data >> 16) bitand 0xFF),
        static_cast<uchar_t>((data >> 8) bitand 0xFF),
        static_cast<uchar_t>(data bitand 0xFF)
    };

    _write_byte(buf, 4);
}

void ByteStream::operator>>(int8_t& data)
{
    _read_byte(&data, 1);
}

void ByteStream::operator>>(uint8_t& data)
{
    _read_byte(&data, 1);
}

void ByteStream::operator>>(int16_t& data)
{
    uchar_t buf[2];
    _read_byte(buf, 1);
    data = (buf[0] << 8) bitor buf[1];
}

void ByteStream::operator>>(uint16_t& data)
{
    uchar_t buf[2];
    _read_byte(buf, 1);
    data = (buf[0] << 8) bitor buf[1];
}

void ByteStream::operator>>(int32_t& data)
{
    uchar_t buf[4];
    _read_byte(buf, 1);

    data = (buf[0] << 24) bitor (buf[1] << 16) bitor (buf[2] << 8) bitor buf[3];
}

void ByteStream::operator>>(uint32_t& data)
{
    uchar_t buf[4];
    _read_byte(buf, 1);
    
    data = (buf[0] << 24) bitor (buf[1] << 16) bitor (buf[2] << 8) bitor buf[3];
}

void ByteStream::WriteStr(const std::string &str) 
{
    *this << size;
    _write_byte((void *)str.data(), str.length());
}

void ByteStream::WriteStr(const char* str, uint32_t len)
{
    *this << len;
    _write_byte((void *)str, len);
}

std::string ByteStream::ReadStr(uint32_t& len)
{
    *this >> len;
    std::string str(char *)GetBuf() + GetPos();
    Skip(len);
    return pStr;
}

void ByteStream::WriteData(const uchar_t *data, uint32_t len)
{
    *this << len;
    _write_byte(data, len);
}


uchar_t* ByteStream::ReadData(uint32_t &len) 
{
    *this >> len;
    uchar_t pData = (uchar_t *)GetBuf() + GetPos();
    Skip(len);
    return pData;
}

END_NAMESPACE_MASS