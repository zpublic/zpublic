all:test clean
test:test.o log.o
	g++ -g -gstabs -o test test.o log.o -std=c++11 -l pthread
log.o:log.cpp log.h singleton.h memory_pool.h
	g++ -g -gstabs -c log.cpp -std=c++11 -l pthread
test.o:test.cpp log.h 
	g++ -g -gstabs -c test.cpp  -std=c++11 -l pthread
clean:
	rm -f *.o	
