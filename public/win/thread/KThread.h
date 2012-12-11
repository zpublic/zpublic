/***************************************************************
 * @date:   2007-10-19
 * @author: BrucePeng
 * @brief:  The encapsulation of thread operation
 *          The base class of other thread class
 *          *Windows OS only*
 */

#ifndef K_THREAD_H_
#define K_THREAD_H_

#include <windows.h>
#include <process.h>

/**
 * @enum ENUM_THREAD_STATE
 * Enumerate the state of a thread
 *
 * enumTHREADS_READY        --- The thread is ready
 * enumTHREADS_EXECUTING    --- The thread is executing
 * enumTHREADS_FINISHED     --- The thread is finished
 */
enum ENUM_THREAD_STATE
{
    enumTHREADS_READY       = 0x01,
    enumTHREADS_EXECUTING   = 0x02,
    enumTHREADS_FINISHED    = 0x03
};

class CThread
{
public:
    // The default constructor
    CThread(void) : m_hThread(NULL),
        m_uThreadID(0), m_nState(enumTHREADS_READY){}

    // The default destructor
    virtual ~CThread(void) {}

public:
    /**
     * @brief Start the thread
     * Underlying thread will be created, then started
     *
     * @return If successfully
     * - true   YES
     *   false  NO
     */
    bool Start(void);

    /**
     * @brief Stop the thread
     * It will wait for underlying thread to stop, then do clean
     *
     * @return If successfully
     * - true   YES
     *   false  NO
     */
    bool Stop(void);

    /**
     * @brief Stop the thread
     * It will wait for underlying thread to stop, then do clean
     *
     * @param[IN]   ulTime      How long to wait, in milliseconds
     *
     * @return If successfully
     * - true   YES
     *   false  NO
     */
    bool StopInTime(IN unsigned long ulTime);
    
    /**
     * @brief Query the state of a thread
     * @return The state identity
     */
    inline int  GetState(void) const;

    /**
     * @brief Get the underlying thread handle
     * @return The thread handle
     */
    inline HANDLE GetHandle(void) const;

    /**
     * @brief Check If a thread is alive
     *
     * @return If alive
     * - true   YES
     *   false  NO
     */
    inline bool IsAlive(void) const;

protected:
    virtual void Run(void)  = 0;

private:
    // The handle of underlying thread
    HANDLE          m_hThread;

    // The identity of underlying thread
    unsigned        m_uThreadID;

    // The state of underlying thread
    int             m_nState;

private:
    // Not support copy constructor and "operator="
    CThread(const CThread&);
    CThread& operator=(const CThread&);

    /**
     * @brief Set the state of thread
     *
     * @param[IN]   nState  The state identity
     */
    inline void SetState(int nState);

    static unsigned __stdcall ThreadFun(void* params);
};

inline bool CThread::IsAlive(void) const
{
    return (NULL == m_hThread);
}

inline int  CThread::GetState(void) const
{
    return m_nState;
}

inline HANDLE CThread::GetHandle(void) const
{
    return m_hThread;
}

inline void CThread::SetState(int nState)
{
    m_nState = nState;
}

#endif // K_THREAD_H_

