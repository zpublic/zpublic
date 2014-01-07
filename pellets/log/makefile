all:test clean
test:test.o log.o
	g++ -g -o test test.o log.o -std=c++11 -l pthread
log.o:log.cpp log.h singleton.h 
	g++ -g -c log.cpp -std=c++11 -l pthread
test.o:test.cpp log.h
	g++ -g -c test.cpp -std=c++11 -l pthread
clean:
	rm -f *.o	
