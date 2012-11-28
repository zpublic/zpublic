/********************************************************************
* @file      : log_macro.hpp
* @author    : zapline <zhuxianzhang@kingsoft.com>
* @date      : 2012/11/27 10:23
* @brief     : 记录日志的宏
* 
* 
*********************************************************************/

#ifndef _LOG_MACRO_HPP_
#define _LOG_MACRO_HPP_

#include "log_queue.hpp"

/*
 * 将需要编译到程序中的最低日志级别定义为宏
 * 例如 DT_LEVEL = 3，
 * 则程序中编译进Error和Fatal
 */
#define DT_LEVEL_TRACE		0
#define DT_LEVEL_INFO		1
#define DT_LEVEL_WARNING	2
#define DT_LEVEL_ERROR		3
#define DT_LEVEL_FATAL		4

///> 需要定义的两个函数

/*
zl::CLogQueue* GetTraceInstance()
{
static zl::CLogQueue mylog("log\\zpublic.log", FALSE);
return &mylog;
}
*/
///> 用于定义日志实例
zl::CLogQueue* GetTraceInstance();


/*
int GetTraceLevel(BOOL bReLoad)
{
static int nLevel = -1;
if (bReLoad || nLevel == -1)
{
}
return nLevel;
}
*/
///> 用于控制运行时日志等级
int GetTraceLevel(BOOL bReLoad = FALSE);


#define DT_LOG(LEVELNAME, LEVELVALUE, h, fmt, ...) \
    if(GetTraceLevel() <= LEVELVALUE && h) \
    (*h).AddLog(LEVELNAME##fmt, __VA_ARGS__)


#if DT_LEVEL<=DT_LEVEL_TRACE
#define DT_TRACE_LOG(h, fmt, ...)   DT_LOG(L"<TRACE>", DT_LEVEL_TRACE, h, fmt, __VA_ARGS__)
#define DT_TRACE(fmt, ...)			DT_TRACE_LOG( GetTraceInstance(), fmt, __VA_ARGS__ )
#define DT_TRACE_LOG_A(h, fmt, ...) DT_LOG("<TRACE>", DT_LEVEL_TRACE, h, fmt, __VA_ARGS__)
#define DT_TRACE_A(fmt, ...)		DT_TRACE_LOG_A( GetTraceInstance(), fmt, __VA_ARGS__ )
#else
#define DT_TRACE_LOG(h, fmt, ...)
#define DT_TRACE(fmt, ...)
#define DT_TRACE_LOG_A(h, fmt, ...)
#define DT_TRACE_A(fmt, ...)
#endif 

#if DT_LEVEL<=DT_LEVEL_INFO
#define DT_INFO_LOG(h, fmt, ...)    DT_LOG(L"<INFO>", DT_LEVEL_INFO, h, fmt, __VA_ARGS__)
#define DT_INFO(fmt, ...)			DT_INFO_LOG( GetTraceInstance(), fmt, __VA_ARGS__  )
#define DT_INFO_LOG_A(h, fmt, ...)  DT_LOG("<INFO>", DT_LEVEL_INFO, h, fmt, __VA_ARGS__)
#define DT_INFO_A(fmt, ...)		DT_INFO_LOG_A( GetTraceInstance(), fmt, __VA_ARGS__ )
#else
#define DT_INFO_LOG(h, fmt, ...)
#define DT_INFO(fmt, ...)
#define DT_INFO_LOG_A(h, fmt, ...)
#define DT_INFO_A(fmt, ...)
#endif 

#if DT_LEVEL<=DT_LEVEL_WARNING
#define DT_WARNING_LOG(h, fmt, ...) DT_LOG(L"<WARN>", DT_LEVEL_WARNING, h, fmt, __VA_ARGS__)
#define DT_WARNING(fmt, ...)		DT_WARNING_LOG( GetTraceInstance(), fmt, __VA_ARGS__  )
#define DT_WARNING_LOG_A(h, fmt, ...)   DT_LOG("<WARN>", DT_LEVEL_WARNING, h, fmt, __VA_ARGS__)
#define DT_WARNING_A(fmt, ...)		    DT_WARNING_LOG_A( GetTraceInstance(), fmt, __VA_ARGS__  )
#else
#define DT_WARNING_LOG(h, fmt, ...)
#define DT_WARNING(fmt, ...)
#define DT_WARNING_LOG_A(h, fmt, ...)
#define DT_WARNING_A(fmt, ...)
#endif 

#if DT_LEVEL<=DT_LEVEL_ERROR
#define DT_ERROR_LOG(h, fmt, ...)   DT_LOG(L"<ERROR>", DT_LEVEL_ERROR, h, fmt, __VA_ARGS__)
#define DT_ERROR(fmt, ...)			DT_ERROR_LOG( GetTraceInstance(), fmt, __VA_ARGS__  )
#define DT_ERROR_LOG_A(h, fmt, ...) DT_LOG("<ERROR>", DT_LEVEL_ERROR, h, fmt, __VA_ARGS__)
#define DT_ERROR_A(fmt, ...)		DT_ERROR_LOG_A( GetTraceInstance(), fmt, __VA_ARGS__  )
#else
#define DT_ERROR_LOG(h, fmt, ...)
#define DT_ERROR(fmt, ...)
#define DT_ERROR_LOG_A(h, fmt, ...)
#define DT_ERROR_A(fmt, ...)
#endif 

#if DT_LEVEL<=DT_LEVEL_FATAL
#define DT_FATAL_LOG(h, fmt, ...)   DT_LOG(L"<FATAL>", DT_LEVEL_FATAL, h, fmt, __VA_ARGS__)
#define DT_FATAL(fmt, ...)          DT_FATAL_LOG( GetTraceInstance(), fmt, __VA_ARGS__  )
#define DT_FATAL_LOG_A(h, fmt, ...) DT_LOG("<FATAL>", DT_LEVEL_FATAL, h, fmt, __VA_ARGS__)
#define DT_FATAL_A(fmt, ...)        DT_FATAL_LOG_A( GetTraceInstance(), fmt, __VA_ARGS__  )
#else
#define DT_FATAL_LOG(h, fmt, ...)
#define DT_FATAL(fmt, ...)
#define DT_FATAL_LOG_A(h, fmt, ...)
#define DT_FATAL_A(fmt, ...)
#endif 

#if _ENABLE_DT 
#define DT                  DT_TRACE
#define DT_A                DT_TRACE_A
#define DT_I                DT_INFO
#define DT_I_A              DT_INFO_A
#define DT_W                DT_WARNING
#define DT_W_A              DT_WARNING_A
#define DT_F                DT_FATAL
#define DT_F_A              DT_FATAL_A
#define DT_E                DT_ERROR
#define DT_E_A              DT_ERROR_A
#else
#define DT(...)
#define DT_A(...)
#define DT_I(...)
#define DT_I_A(...)
#define DT_W(...)
#define DT_W_A(...)
#define DT_F(...)
#define DT_F_A(...)
#define DT_E(...)
#define DT_E_A(...)
#endif

#endif