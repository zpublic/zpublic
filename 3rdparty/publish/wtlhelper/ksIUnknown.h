#ifndef __kis_iunknown__
#define __kis_iunknown__
#pragma once
//实现IUnknown 几个恶心的函数 xujianxin

class ksIUnknown
{
public:
	ksIUnknown():m_cRef(0)
	{
	}
protected:
	int m_cRef;
};


#define IMP_BEGIN virtual ULONG __stdcall AddRef()\
{\
	return ++m_cRef;\
}\
virtual ULONG __stdcall Release()\
{\
	--m_cRef;\
	if(m_cRef == 0 )\
	{\
		delete this;\
	}\
	return m_cRef;\
}\
virtual HRESULT __stdcall QueryInterface(REFIID iid,void ** ppvObject)\
{\
	if (ppvObject == NULL)\
	{\
		return E_INVALIDARG;\
	}\
	*ppvObject = NULL;\
	if (0)\
		return E_NOTIMPL;		

#define IMP_HERE(x1,x2)	\
	else if (iid == x1::IID_ ## x2)\
		*ppvObject = dynamic_cast<x1 ## :: ## x2 ## *>(this);

#define IMP_NONHERE(x)	\
	else if (iid == IID_ ## x)\
		*ppvObject = dynamic_cast<x ## *>(this);

#define IMP_END	\
	if (*ppvObject)\
		AddRef();\
	else\
		return E_NOTIMPL;\
	return S_OK;\
}




//sample
/*
class IXXXYY : public XXX::I1,
			   public XXX::I2,
			   public XXX::I3,
			   public ksIUnknown                            
{
public:
IMP_BEGIN
IMP_HERE(XXX, I1)
IMP_HERE(XXX, I2)
IMP_HERE(XXX, I3)
IMP_END
public:
*/


#endif //__kis_iunknown__