#ifndef COMMON_H_
#define COMMON_H_

#ifdef _WIN32
	#define SERVER_DECL __declspec(dllexport)
#endif

//types
#ifndef byte
    typedef unsigned char byte;
#endif

#endif