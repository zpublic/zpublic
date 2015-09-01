/*************************************************************************
*                                                                       *
*  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
*  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
*        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
*       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
*     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
*    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
*   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
* !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
*                                                                       *
*               Website: https://github.com/zpublic/zpublic             *
*                                                                       *
************************************************************************/

/**
* @file
* @brief buffer类
*/

#pragma once

namespace zl
{
namespace WinUtils
{
    /**
     * @brief buffer类
     */
    class ZLBuffer
    {
    public:
        ZLBuffer()
        {
            m_pBuf=NULL;
            m_bufSize=0;
            m_attached = FALSE;
        }
        ~ZLBuffer()
        {
            _free();
        }
        typedef unsigned char byte;
    public:
        /**
         * @brief 分配buffer
         * @param[in] size 字节大小
         * @return buffer地址
         */
        byte* Alloc(DWORD size)
        {
            void *pTemp = realloc(m_pBuf,size+1);
            if (pTemp != NULL)
            {
                m_pBuf = (BYTE*)pTemp;
                m_bufSize = size;
                m_pBuf[size]=0;
            }
            return m_pBuf;
        }
        
        /**
         * @brief 释放buffer并清空信息
         */
        VOID Clear()
        {
            _free();
        }
        
        /**
         * @brief 获取buffer地址
         * @return buffer地址
         */
        byte* GetBuffer() const
        {
            return m_pBuf;
        }
        
        /**
         * @brief 获取buffer大小
         * @return buffer字节大小
         */
        DWORD GetSize() const
        {
            return m_bufSize;
        }

        /**
         * @brief 拷贝数据到当前buffer
         * @param[in] src 数据起始地址
         * @param[in] len 要拷贝的数据字节长度
         * @return buffer地址
         */
        const byte* Copy(const void* src,DWORD len)
        {
            Alloc(len);
            if (m_pBuf)
            {
                ::memcpy(m_pBuf,src,len);
                m_bufSize=len;
            }
            return *this;
        }
        
        /**
         * @brief 拷贝数据到当前buffer
         * @param[in] src 数据源
         * @return buffer地址
         */
        const byte* Copy(const ZLBuffer& src)
        {
            return Copy(src,src.GetSize());
        }

        /**
         * @brief 托管一段buffer，在类对象尝试释放内存时会调用free
         * @param[in] src 数据源
         */
        VOID Attach(void* buffer,size_t bufsize)
        {
            assert(buffer);
            if (buffer && bufsize)
            {
                _free();
                m_attached = TRUE;
                m_pBuf = (BYTE*)buffer;
                m_bufSize = (DWORD)bufsize;
            }
        }

        /**
         * @brief 解除托管一段buffer，会触发Clear
         */
        VOID Detach()
        {
            _free();
        }

        /**
         * @brief 将数据写入到文件
         * @param[in] filePath 文件路径
         * @return 成功返回TRUE，失败返回FALSE
         */
        BOOL WriteToFile(LPCTSTR filePath) const
        {
            FILE* fp=NULL;
            _tfopen_s(&fp,filePath,TEXT("wb"));
            if (fp)
            {
                size_t temp;
                temp = fwrite(GetBuffer(),GetSize(),1,fp);
                assert(temp == 1);
                fclose(fp);
                return TRUE;
            }
            return FALSE;
        }

        /**
         * @brief 填充字节数据，类似memset
         * @param[in] value 填充值
         */
        void Fill(byte value=0)
        {
            ::memset(m_pBuf,value,m_bufSize);
        }
        
        /**
         * @brief 将数据写入到指定内存
         * @param[in] p 写入的起始地址
         * @param[in] size 写入大小
         * @param[in] startPos 要写入的buffer的起始偏移
         * @return 实际写入的数据大小
         */
        DWORD Write(void* p,DWORD size,DWORD startPos=0)
        {
            if (startPos >= m_bufSize || p==NULL || m_pBuf==NULL)
            {
                return 0;
            }
            if (startPos + size >= m_bufSize)
            {
                size = m_bufSize-startPos-1;
            }
            if (size)
            {
                ::memcpy(m_pBuf+startPos,p,size);
            }
            return size;
        }

        operator const BYTE* () const
        {
            return m_pBuf;
        }
    private:
        void operator=(const ZLBuffer &src);
        void _free()
        {
            if (m_attached)
            {
                m_attached = FALSE;
                m_pBuf = NULL;
                m_bufSize = 0;
                return;
            }
            if(m_pBuf)
            {
                free(m_pBuf);
                m_pBuf = NULL;
                m_bufSize = 0;
            }
        }
    protected:
        BYTE* m_pBuf;
        DWORD m_bufSize;
        BOOL m_attached;
    };

    /**
     * @brief 读写buffer,固定size大小
     */
    class ZLReadWriteBuffer : public ZLBuffer
    {
    public:
        ZLReadWriteBuffer()
        {
            m_writePos = 0;
            m_readPos = 0;
        }
        ~ZLReadWriteBuffer(){};
    public:
        /**
         * @brief 写入数据
         * @param[in] pData 读取的起始地址
         * @param[in] dataSize 写入大小
         * @return 实际写入的数据大小
         */
        DWORD Append(const void *pData,DWORD dataSize)
        {
            if (m_writePos >= m_bufSize || pData==NULL || m_pBuf==NULL)
            {
                return 0;
            }
            if (m_writePos+dataSize > m_bufSize)
            {
                dataSize = m_bufSize-m_writePos-1;
            }
            if (dataSize)
            {
                ::memcpy(m_pBuf+m_writePos,pData,dataSize);
                m_writePos += dataSize;
            }
            return dataSize;
        }

        /**
         * @brief 读取数据
         * @param[in] pOut 写入的起始地址
         * @param[in] bytesToRead 写入大小
         * @return 实际读取到的数据大小
         */
        DWORD Get(void *pOut,DWORD bytesToRead)
        {
            if (m_pBuf==NULL || pOut==NULL || bytesToRead==0)
            {
                return 0;
            }
            if (bytesToRead+m_readPos >= m_writePos)
            {
                bytesToRead = m_writePos-m_readPos;
            }
            if (bytesToRead)
            {
                ::memcpy(pOut,GetBuffer() + m_readPos,bytesToRead);
                m_readPos += bytesToRead;
            }
            return bytesToRead;
        }
        
        /**
         * @brief 初始化写入指针到起始位置
         */
        void ResetWritePos()
        {
            m_writePos = 0;
        }
        
        /**
         * @brief 初始化读取指针到起始位置
         */
        void ResetReadPos()
        {
            m_readPos = 0;
        }
    private:
        void operator =(const ZLReadWriteBuffer &src);
        DWORD m_writePos;
        DWORD m_readPos;
    };
}
}