#ifndef ZPUBLIC_JSON_PARSER_IPC_RUN
#define ZPUBLIC_JSON_PARSER_IPC_RUN

/*
�ӿڲ���֧����������
    1.�������ͣ�char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long
                float, double
    2.��׼�⣺string&, wstring&, list&, vector&(����list, vector �ȵ�ģ�����������֧�ֵ�����)
    3.�ַ�����const char*, const wchar_t*

�ṹ���Ա֧���������ͣ�
    1.�������ͣ�char, unsigned char, short, unsigned short, int, unsigned int, long, unsigned long
                float, double
    2.��׼�⣺string, wstring, list, vector(����list, vector �ȵ�ģ�����������֧�ֵ�����)
    3.�ַ�����char[], wchar_t[]


ע���
    1.����һ��������ȫ�ֺ����������£�
    int PrintValue(int v);
    ADD_GLOBAL_FUNC_1(PrintValue, int);	//��PrintValue���뺯����

    2.����һ�������ӿڵ��ຯ���������£�
    class IExport
    {
        DECLARE_CLASS_CREATE_FUNC(IExport);��//��������ľ�̫��������
    public:
        int PrintValue(int v);
    }
    DECLARE_EXPORT_INTERFACE(IExport); //��IExport���봴������������ͨ������������������
    ADD_CLASS_FUNC_1(IExport, PrintValue, int);

    4.�ӿڲ����Ľṹ������и����������磺
    struct Param
    {
        int n;
        std::string str;
        std::list<int> lst;
    }
    DECLARE_JSON_CONVERSION_3(Param, n, str, lst);//����Param��Jsonת������

    5.����ȫ�ֺ���ʱֱ�Ӵ���Json�ַ������磺
    std::string json_str = CreateGlobalCall("PrintValue", 3888);
    zl::Ipc::ipcReturnParse parse = ipcRemoteRunManager::Inst().Run(json_str.c_str());
    int n_ret_value = -1;
    if (parse.Value(n_ret_value)) {...};use n_ret_value
    else {...}error

    6.���ýӿڵ��ຯ��ʱ��Ҫ�ȴ���������ͨ������ID���ã��磺
    std::string json_str = CreateObjectCreateCall("IExport");
    zl::Ipc::ipcReturnParse parse = ipcRemoteRunManager::Inst().Run(json_str.c_str());
    parse.Value(n_id);

    json_str = CreateClass(n_id, "PrintValue", 3888);
    int n_ret = ipcRemoteRunManager::Inst().Run(json_str.c_str());

    7.C++����ʱ����ipcJsonParam���������ú�����Html�͵��Լ��齨Json�ַ���

    8.����Json�ַ���˵��
      ����ȫ�ֺ���ʱ��Json�ַ���
        {"func_name":"PrintValue","operation":"run","param_array":[3888]}

      �����ຯ��ʱ��Json�ַ���
        {"func_name":"PrintValue","id":1,"operation":"run","param_array":[100]}

      ���÷�����ʵ��ʱ��Json�ַ���
        {"class_name":"IExport","operation":"alloc"}

      �����ͷ���ʵ��ʱ��Json�ַ���

        {"id":1,"operation":"dealloc"}

      ���ص�Json�ַ���
        {"code":0,"message":"succeed","value":1}


*/

/*
δ��ɣ�
    1.����std::map֧��
    2.����ע��

    3.��Call�ĵ��ü�һ�㣬���£�
    ipcGlobalRun_0<class R>
    {
        ipcGlobalRun_0(const char* class_name);
        void Run();
        int RetCode();
        std::string& RetMsg();
        R& RetValue();
    }
    ipcGlobalRun_1<class R, class P1>;


*/
#pragma warning(push)

#pragma warning( disable : 4244 ) //�ر�����ת����ʧ���棬��doubleת��int
#pragma warning( disable : 4018 ) //�رձȽϲ����������Ͳ�һ�����棬��int > double
#pragma warning( disable : 4996 ) //�ر�C�⺯������ȫ�ľ��棬��strcpy, wcscpy��

#include "ipcjsonconvert.h"
#include "ipcjsonparam.h"
#include "ipcfuncmgr.h"

#pragma warning(pop)

#endif