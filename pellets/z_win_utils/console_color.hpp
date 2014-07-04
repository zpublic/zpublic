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
        emConsoleColourBlack            = 0,    ///>0= 黑色
        emConsoleColourBlue             = 1,    ///>1= 蓝色
        emConsoleColourGreen            = 2,    ///>2= 绿色
        emConsoleColourLightBlue        = 3,    ///>3= 湖蓝色
        emConsoleColourRed              = 4,    ///>4= 红色
        emConsoleColourPurple           = 5,    ///>5= 紫色
        emConsoleColourYellow           = 6,    ///>6= 黄色
        emConsoleColourWhite            = 7,    ///>7= 白色
        emConsoleColourGray             = 8,    ///>8= 灰色
        emConsoleColourBabyBlue         = 9,    ///>9= 淡蓝色
        emConsoleColourPaleGreen        = 10,   ///>A= 淡绿色
        emConsoleColourPaleLightGreen   = 11,   ///>B= 淡浅色
        emConsoleColourPompadour        = 12,   ///>C= 淡红色
        emConsoleColourLavender         = 13,   ///>D= 淡紫色
        emConsoleColourFaintYellow      = 14,   ///>E= 淡黄色
        emConsoleColourBrightWhite      = 15,   ///>F= 亮白色
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