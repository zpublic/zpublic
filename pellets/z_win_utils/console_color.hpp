/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    typedef enum _emConsoleColor
    {
        emConsoleColourBlack = 0,
        emConsoleColourYellow = 6,
        emConsoleColourGray = 8, 
//         0=黑色          8=灰色
//         1=蓝色          9=淡蓝色
//         2=绿色          A=淡绿色
//         3=湖蓝色        B=淡浅绿色
//         4=红色          C=淡红色
//         5=紫色          D=淡紫色
//         6=黄色          E=淡黄色
//         7=白色          F=亮白色 
    }emConsoleColor;

    class ZLConsoleColor
    {
    public:
        ZLConsoleColor() {}
        ~ZLConsoleColor() {}

    public:
        static BOOL SetConsoleForegroundColor(emConsoleColor color)
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                WORD wColor = GetConsoleColor() & 0xf0;
                return ::SetConsoleTextAttribute(hOutPutHandle, wColor | (BYTE)color);
            }
            return FALSE;
        }

        static BOOL SetConsoleBackgroundColor(emConsoleColor color)
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                WORD wColor = GetConsoleColor() & 0xf;
                return ::SetConsoleTextAttribute(hOutPutHandle, wColor | ((WORD((BYTE)color) << 4)));
            }
            return FALSE;
        }

        static BOOL SetColorFontDefault()
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                return ::SetConsoleTextAttribute(hOutPutHandle, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
            }
            return FALSE;
        }

        static WORD GetConsoleColor()
        {
            WORD wRet = 0;
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                CONSOLE_SCREEN_BUFFER_INFO consoleInfo = {0};
                if (::GetConsoleScreenBufferInfo(hOutPutHandle, &consoleInfo))
                {
                    wRet = consoleInfo.wAttributes;
                }
            }
            return wRet;
        }
    };

}
}