#include "log.h"

#if defined(_WIN32)
#define  SPRINTF( buffer, sizeOfbuffer, fmt, ... )\
    sprintf_s( buffer, sizeOfbuffer, fmt, ##__VA_ARGS__ )
#else
#define SPRINTF( buffer, sizeOfbuffer, fmt, ... )\
	snprintf( buffer, sizeOfbuffer, fmt, ##__VA_ARGS__ )
#endif


Log::Log()
{
	//	_thread_ptr = std::shared_ptr<std::thread>( new std::thread( std::bind( &Log::output_log, this ) ) );
}

void Log::init( const std::string& log_name )
{
	flag = true;

	_log_file.open( log_name.c_str(), std::ios::app );
	_thread_ptr = std::shared_ptr<std::thread>( new std::thread( &Log::output_log, this ) );
}

Log::~Log()
{
	//最后一次唤醒线程,然后等待IO线程退出
	flag = false;
	_cv.notify_one();
	_thread_ptr->join();

	read_log();
	_log_file.close();
}

char* Log::format_log( char* fmt, ... )
{
	va_list args;
	va_start( args, fmt );

	_pool_mutex.lock();
	char* buff = static_cast<char *>( _pool.acquire() );
	_pool_mutex.unlock();

#if defined(_WIN32)
	vsprintf_s( buff, 1024, fmt, args );
#else
	vsnprintf( buff, 1024, fmt, args );
#endif
	va_end( args );

	return buff;
}

std::string Log::get_file_name( const char* path_file_name )
{
	std::string file_name( path_file_name );
	std::string::size_type idx;
	idx = file_name.find_last_of( "\\/" );
	if( idx == std::string::npos )
	{
		return file_name;
	}
	else
	{
		return file_name.substr( idx + 1 ); 
	}
}

void Log::write( char* msg, Priority prio, const char* file, int line )
{
	time_t t;
	struct tm local_time;
	t = time( NULL );
#if defined(_WIN32)
	localtime_s( &local_time, &t );
#else
	localtime_r( &t, &local_time );
#endif
	_pool_mutex.lock();
	char *buff = static_cast<char *>( _pool.acquire() );
	char *log_message = static_cast<char *>( _pool.acquire() );
	_pool_mutex.unlock();
	strftime( buff, 1024, "%Y-%m-%d %H:%M:%S", &local_time );

	switch( prio )
	{
	case PRIO_FATAL:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Fatal : %s", buff, file, line, msg );	
			break;
		}
	case PRIO_CRITICAL:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Critical : %s", buff, file, line, msg );	
			break;
		}
	case PRIO_ERROR:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Error : %s", buff, file, line, msg );	
			break;
		}
	case PRIO_WARNING:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Warning : %s", buff, file, line, msg );	
			break;
		}
	case PRIO_NOTICE:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Notice : %s", buff, file, line, msg );	
			break;
		}
	case PRIO_INFORMATION:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Information : %s", buff, file, line, msg );	
			break;
		}
	case PRIO_DEBUG:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Debug : %s", buff, file, line, msg );
			break;
		}
	case PRIO_TRACE:
		{
			SPRINTF( log_message, 1024, "[ %s %s(%d) ] Trace : %s", buff, file, line, msg );	
			break;
		}
	default:
		break;
	}

	SPRINTF( msg, 1024, "%s", log_message );

	_pool_mutex.lock();
	_pool.release( log_message );
	_pool.release( buff );
	_pool_mutex.unlock();

	std::unique_lock<std::mutex> u_lock(_mutex);
	_work_queue.push( msg );

	_cv.notify_one();
}

void Log::output_log()
{
	std::unique_lock<std::mutex> u_lock(_mutex);

	while( true )
	{
		if(flag)
		{
			_cv.wait(u_lock);
			read_log();
		}
		else
		{
			break;
		}
	}
}

void Log::read_log()
{
	char *buffer;
	while( !_work_queue.empty() )
	{
		buffer = _work_queue.front();
		Log::_work_queue.pop();

		//输出日志信息
		_log_file<<buffer<<std::endl;
		std::cout<<buffer<<std::endl;
		//回收内存
		_pool.release(buffer);
	}
}

void Log::fatal( char* msg, const char* file, int line )
{
	write( msg, PRIO_FATAL, file, line );
}

void Log::critical( char* msg, const char* file, int line )
{
	write( msg, PRIO_CRITICAL, file, line );
}

void Log::error( char* msg, const char* file, int line )
{
	write( msg, PRIO_ERROR, file, line );
}

void Log::warning( char* msg, const char* file, int line )
{
	write( msg, PRIO_WARNING, file, line );
}

void Log::notice( char* msg, const char* file, int line )
{
	write( msg, PRIO_NOTICE, file, line );
}

void Log::information( char* msg, const char* file, int line )
{
	write( msg, PRIO_INFORMATION, file, line );
}

void Log::debug( char* msg, const char* file, int line )
{
	write( msg, PRIO_DEBUG, file, line );
}

void Log::trace( char* msg, const char* file, int line )
{
	write( msg, PRIO_TRACE, file, line );
}