#include "stdafx.h"
#include "test_xde.h"

void test_xde()
{
    xde_instr instr = {0};

    /*
    6A 00       push  0
    */
    const char* pszOpCode = "\x6A\x00";

    /* returns:                                                         */
    /*   0 if error                                                     */
    /*   instruction length (== diza.len) if OK                         */
    xde_disasm((unsigned char *)pszOpCode, &instr);
    assert(instr.len == 2);

    /* returns: diza.len                                                */
    unsigned char szOpCode[256] = {0};
    int nLen = xde_asm(szOpCode, &instr);
    assert(nLen == 2);
}

