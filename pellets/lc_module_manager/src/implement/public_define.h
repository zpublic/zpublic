#pragma once

// ������ϢID
enum MsgDefine
{
    Msg_Reserve_Begin,
    Msg_Module_Initialzed,
    Msg_Reserve_End = 50,                    // ������ϢID�ķֽ��ߣ�С�ڴ˷ֽ��ߵ���ϢΪ������Ϣ(ͨ����Ϣ��ϵͳ��Ϣ�ȵ�)
                                    
    Msg_Test_Begin = 100,                    // ģ�鶨����ϢIDʱ�����������г��ֽ���
    Msg_Test_1 = 123,
    Msg_Test_2,
    Msg_Test_End = 150,
};

// ����ģ��ID
enum ModuleDefine                            //ÿ������һ��ģ��ʱ��Ҫ�ڴ˶���ģ���ID
{
    Module_ALL,
    Module_ModuleCenter,

    Module_Define_begin = 50,                    // ֮ǰ��ֵ�Ǳ���ֵ��ģ�鶨��Ӵ˿�ʼ
    Module_ModuleSample,

};