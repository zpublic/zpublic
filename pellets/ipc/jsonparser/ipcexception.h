#ifndef ZPUBLIC_JSON_PARSER_IPC_EXCEPTION
#define ZPUBLIC_JSON_PARSER_IPC_EXCEPTION

#include <exception>
#include "ipclocation.h"

namespace zl
{
namespace Ipc
{

class ipcException : public std::exception
{
public:
	ipcException() : exception(){}
	ipcException(const char *const& s) : exception(s){error_msg_ = s;}
	ipcException(ipcLocation& l) : location_(l){
		error_msg_ = l.File();
		error_msg_ += std::string(" ") + l.Func();
		error_msg_ += std::string(" ") + l.Line();
	}
	virtual ~ipcException(){}

	virtual const char * __CLR_OR_THIS_CALL what() const
	{
		return error_msg_.c_str();
	}

	ipcLocation& Location(){return location_;}
	
protected:
	std::string error_msg_;
	ipcLocation location_;
};

}
}

#endif