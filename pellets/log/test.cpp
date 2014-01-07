#include"log.h"
#include <thread>
#include <memory>
#include <functional>
void debug()
{
	int count = 0;
	while( count<500 )
		debug_log( "just for testing debug %d", ++count );
}

void error()
{
	int count = 0;
	while( count < 500 )
		error_log( "just for testing error %d", ++count );
}

void trace()
{
	int count = 0;
	while( count < 500 )
		trace_log( "just for testing trace %d", ++count );
}

void info()
{
	int count = 0;
	while( count < 500 )
		info_log( "just for testing info %d", ++count );
}

int main()
{
	Log::getInstance().init("server.log");

	std::thread t1( debug );
	std::thread t2( error );
	std::thread t3( trace );
	std::thread t4( info );

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	fatal_log("success");
	/*
	int  count = 0;
	while( count<100000 )
	{
		++count;
		debug_log("just for server log test %d", count );
//		std::this_thread::sleep_for( std::chrono::seconds(1) );
	}
*/
	return 0;
}

