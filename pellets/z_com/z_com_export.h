/***********************
*!!!!!!!!!!!!!!!!!!!!!!*
*!!!!Do not Modify!!!!!*
*!!!!!!!!!!!!!!!!!!!!!!*
************************
*如果你要修改任何相关代码，或者有什么建议，请提交到
*https://github.com/zpublic/zpublic
********************************************************/
#pragma once

/*

//////////////////////////////////////////////////////////////////////////

完成类似COM的接口导出，需要做以下三步

//////////////////////////////////////////////////////////////////////////

(1)
ZL_BEGIN_COM_MAP( CXXX )
    ZL_COM_INTERFACE_ENTRY( IXXX )
    或
    ZL_COM_INTERFACE_ENTRY_IID( IXXX )
ZL_END_COM_MAP()

用于实现IUnknown:
这组宏需要加入到你实现接口的类里面, 帮你实现IUnknown的3个虚函数,

例:
假如接口声明如下:
struct __declspec( uuid( "7CCB588B-6746-47f4-9509-DDF3A4637A63" ) )
ICompute: public IUnknown
{
public:
virtual int __stdcall Add( int a, int b ) = 0;
virtual int __stdcall Sub( int a, int b ) = 0;
};
你的实现: (只需要实现需要的接口, 再加入该组宏, 帮你实现IUnknown)
class KCompute : public ICompute
{
public:
    virtual int __stdcall Add( int a, int b )
    {
        return a + b;
    }
    virtual int __stdcall Sub( int a, int b )
    {
        return a - b;
    }
    ZL_BEGIN_COM_MAP( KCompute )
        ZL_COM_INTERFACE_ENTRY( ICompute )
    ZL_END_COM_MAP()
};

//////////////////////////////////////////////////////////////////////////

(2)
ZL_BEGIN_OBJECT_MAP()
    ZL_OBJECT_ENTRY( IXXX, CXXX )
ZL_END_OBJECT_MAP()

DllGetClassObject的实现
我们的简单COM规范不使用CLSID, 不使用类厂, 只需要通过一个接口ID从DLL中创建
出实例, 而接口ID和其创建的实例的对应关系需要该组宏来指定:

例:
ZL_BEGIN_OBJECT_MAP()
    ZL_OBJECT_ENTRY( ICompute, KCompute )
ZL_END_OBJECT_MAP()

//////////////////////////////////////////////////////////////////////////

(3)
ZL_USE_DEFAULT_COM_PROC
或
ZL_USE_DEFAULT_COM_PROCEX

前者实现了 DllCanUnloadNow 和 DllGetClassObject, 
后者实现了 DllCanUnloadNow 和 DllGetClassObject 和 DllMain
省去你手工写这写函数.

你也可以不使用这两个宏, 自己实现上述函数

//////////////////////////////////////////////////////////////////////////

最后, 不要忘了写.def文件
LIBRARY      "xxxmodule"
EXPORTS
DllGetClassObject   PRIVATE
DllCanUnloadNow     PRIVATE

*/

#include "export/z_com_dll_object.hpp"
#include "export/z_com_dll_module.hpp"


//////////////////////////////////////////////////////////////////////////

#define ZL_BEGIN_COM_MAP( CComImpl ) HRESULT _InternalQueryInterface( REFIID riid, void **ppvObject ) { \
    if( ppvObject == NULL ) return E_INVALIDARG; \
    HRESULT hr = E_FAIL; \
    if( false) {}

#define ZL_COM_INTERFACE_ENTRY( I ) 	 else if( riid == __uuidof( I ) || riid == IID_IUnknown ) { \
    *ppvObject = static_cast<I*>(this); \
    hr = S_OK; } 

#define ZL_COM_INTERFACE_ENTRY_IID( I )  else if( riid == IID_##I || riid == IID_IUnknown ) { \
    *ppvObject = static_cast<I*>(this); \
    hr = S_OK; } 

#define ZL_END_COM_MAP()  else { \
    *ppvObject = NULL; \
    hr = E_NOINTERFACE; } \
    return hr; }

//////////////////////////////////////////////////////////////////////////

#define ZL_BEGIN_OBJECT_MAP() HRESULT ZL_DllGetClassObject( REFIID riid, LPVOID* ppv ){ \
    if( ppv == NULL ) return E_INVALIDARG;

#define ZL_OBJECT_ENTRY( IXXX, CXXX ) if( riid == __uuidof( IXXX ) ){ \
    IXXX* pI = new ZLComDllObject<CXXX>; \
    if( pI == NULL ) return E_OUTOFMEMORY; \
    pI->AddRef(); \
    *ppv = pI; \
    return S_OK; }

#define ZL_OBJECT_ENTRY_IID( IXXX, CXXX ) if( riid == IID_##IXXX ) ){ \
    IXXX* pI = new ZLComDllObject<CXXX>; \
    if( pI == NULL ) return E_OUTOFMEMORY; \
    pI->AddRef(); \
    *ppv = pI; \
    return S_OK; }

#define ZL_END_OBJECT_MAP() *ppv = NULL; \
    return E_NOINTERFACE; }

//////////////////////////////////////////////////////////////////////////

#define ZL_USE_DEFAULT_COM_PROC  ZLComDllModule _Module; \
    STDAPI  DllCanUnloadNow(void){  \
    return _Module.DllCanUnloadNow();}  \
    STDAPI  DllGetClassObject(IN REFCLSID rclsid, IN REFIID riid, OUT LPVOID FAR* ppv){  \
    return _Module.DllGetClassObject( rclsid, riid, ppv );}


#define ZL_USE_DEFAULT_COM_PROCEX    ZL_USE_DEFAULT_COM_PROC \
    BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved){ \
    if( ul_reason_for_call == DLL_PROCESS_ATTACH ) _Module.Init( hModule ); \
    return TRUE; }
