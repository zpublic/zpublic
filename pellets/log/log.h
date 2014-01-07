#ifndef _LOG_H_
#define _LOG_H_


#include <iostream>
#include <queue>
#include <stdio.h>
#include <stdarg.h>
#include "memory_pool.h"
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <fstream>
#include <mutex>
#include <ctime>
#include <condition_variable>
#include "singleton.h"

class Log : public Singleton<Log>
{
    enum Priority
	{
		PRIO_FATAL = 1,
		PRIO_CRITICAL,
		PRIO_ERROR,
		PRIO_WARNING,
		PRIO_NOTICE,
		PRIO_INFORMATION,
		PRIO_DEBUG,
		PRIO_TRACE
	};
public:
	Log();
	~Log();

public:
	void fatal( char* msg, const char* file, int line );
	void critical( char* msg, const char* file, int line );
	void error( char* msg, const char* file, int line );
	void warning( char* msg, const char* file, int line );
	void notice( char* msg, const char* file, int line );
	void information( char* msg, const char* file, int line );
	void debug( char* msg, const char* file, int line );
	void trace( char* msg, const char* file, int line );

public:
	void init( const std::string& log_name );
	char *format_log( char *format, ... );
	std::string get_file_name( const char* path_file_name );

private:
	void write( char *msg, Priority prio, const char* file, int line );
	void output_log();
	void read_log();

private:
	bool    flag;         
	std::ofstream    _log_file;
	MemoryPool       _pool;
	std::mutex       _pool_mutex;
	std::shared_ptr<std::thread>  _thread_ptr;
	std::queue<char *> _work_queue;  

	std::mutex         _mutex;       
	std::condition_variable  _cv;    
};

#define __G_LOGGER__\
	Log::getInstance()

#define __FORMAT__( fmt, ... )\
	Log::getInstance().format_log( fmt, ##__VA_ARGS__ )

#define __FILE_NAME__\
	__G_LOGGER__.get_file_name( __FILE__ ).c_str()

#define debug_log( fmt, ... )\
	__G_LOGGER__.debug( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define fatal_log( fmt, ... )\
	__G_LOGGER__.fatal( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define critical_log( fmt, ... )\
	__G_LOGGER__.critical( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define error_log( fmt, ... )\
	__G_LOGGER__.error( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define warning_log( fmt, ... )\
	__G_LOGGER__.warning( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define notice_log( fmt, ... )\
	__G_LOGGER__.notice( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define info_log( fmt, ... )\
	__G_LOGGER__.information( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#define trace_log( fmt, ... )\
	__G_LOGGER__.trace( __FORMAT__( fmt, ##__VA_ARGS__ ), __FILE_NAME__, __LINE__ )

#endif