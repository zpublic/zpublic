#pragma once
#include "SavesDef.h"

///> ¥Êµµπ‹¿Ì¿‡
class CSavesMgr
{
public:
    CSavesMgr();
    ~CSavesMgr();

public:
    bool Load();

    int Save();
};

