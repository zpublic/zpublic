
#include"log.h"
#include <thread>
#include <memory>
#include <functional>

void debug()
{
	int count = 0;
	while( count<500 )
	{
		wchar_t test[]=L"just for testing debug";
		++count;
		debug_log( L"%ls %d", test, count );
	}
}

void error()
{
	int count = 0;
	while( count < 500 )
	{
		char test[] = "just for testing error";
		error_log( "%s %d", test, ++count );
	}
}

void trace()
{
	int count = 0;
	while( count < 500 )
	{
		char test[] = "just for testing trace";
		trace_log( "%s %d", test, ++count );
	}
}

void info()
{
	int count = 0;
	while( count < 500 )
		info_log( "just for testing info %d", ++count );
}

int main()
{
	Log::getInstance().init(std::string("server.log"));

	std::thread t1( debug );
	std::thread t2( error );
	std::thread t3( trace );
	std::thread t4( info );

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	fatal_log(L"success");

	return 0;
}
