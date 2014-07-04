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

#define FOREGROUND_WHITE (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED)
#define BACKGROUND_WHITE (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED)

namespace zl
{
namespace WinUtils
{

    class ZLConsoleColour
    {
    public:
        ZLConsoleColour() {}
        ~ZLConsoleColour() {}

    public:
        static BOOL SetConsoleColor(WORD wConsoleColorType = FOREGROUND_WHITE)
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                return ::SetConsoleTextAttribute(hOutPutHandle, wConsoleColorType);
            }
            return FALSE;
        }

        static BOOL SetColorFontGreen()
        {
            return SetConsoleColor(FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        }

        static BOOL SetColorFontRed()
        {
            return SetConsoleColor(FOREGROUND_INTENSITY | FOREGROUND_RED);
        }

        static BOOL SetColorFontDefault()
        {
            return SetConsoleColor(FOREGROUND_WHITE);
        }

        static BOOL SetColorFontBlue()
        {
            return SetConsoleColor(FOREGROUND_INTENSITY | FOREGROUND_BLUE);
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

        static BOOL AppendConsoleColor(WORD wConsoleColorType)
        {
            HANDLE hOutPutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOutPutHandle != INVALID_HANDLE_VALUE)
            {
                return ::SetConsoleTextAttribute(hOutPutHandle, GetConsoleColor() | wConsoleColorType);
            }
            return FALSE;
        }
    };

}
}