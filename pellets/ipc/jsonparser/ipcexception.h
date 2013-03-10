#ifndef ZPUBLIC_JSON_PARSER_IPC_EXCEPTION
#define ZPUBLIC_JSON_PARSER_IPC_EXCEPTION

#include <exception>
#include "ipclocation.h"

namespace zl
{
namespace Ipc
{

class KException : public std::exception
{
public:
	KException() : exception(){}
	KException(const char *const& s) : exception(s){error_msg_ = s;}
	KException(KLocation& l) : location_(l){
		error_msg_ = l.File();
		error_msg_ += std::string(" ") + l.Func();
		error_msg_ += std::string(" ") + l.Line();
	}
	virtual ~KException(){}

	virtual const char * __CLR_OR_THIS_CALL what() const
	{
		return error_msg_.c_str();
	}

	KLocation& Location(){return location_;}
	
protected:
	std::string error_msg_;
	KLocation location_;
};

}
}

#endif