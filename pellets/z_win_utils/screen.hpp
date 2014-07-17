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

/**
 * @file
 * @brief 屏幕相关
 * 
 *      屏幕分辨率设置等.
 */


#pragma once
#include "win_utils_header.h"

namespace zl
{
namespace WinUtils
{
    /**
     * @brief 屏幕分辨率设置
     */
    class ZLScreen
    {
    public:
        /**
         * @brief 获取屏幕宽高
         * @param[out] width  宽
         * @param[out] height 高
         * @see GetSystemMetrics
         */
        static void GetScreenResolution(int& width, int& height)
        {
            width = ::GetSystemMetrics(SM_CXSCREEN);
            height = ::GetSystemMetrics(SM_CYSCREEN);
        }
        /**
         * @brief 设置屏幕宽高
         * @param[in] width      宽
         * @param[in] height     高
         * @param[in] BitsPerPel 每像素的比特数
         * @return 成功返回TRUE，失败返回FALSE
         * @see ChangeDisplaySettings
         */
        static BOOL SetScreenResolution(int width, int height, int BitsPerPel = 32)
        {
            DEVMODE lpDevMode;
            lpDevMode.dmBitsPerPel  = 32;
            lpDevMode.dmPelsWidth   = width;
            lpDevMode.dmPelsHeight  = height;
            lpDevMode.dmSize        = sizeof(lpDevMode);
            lpDevMode.dmFields      = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
            LONG result = ::ChangeDisplaySettings(&lpDevMode,0);
            if (result == DISP_CHANGE_SUCCESSFUL)
            {
                ::ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);
                return TRUE;
            }
            else
            {
                ::ChangeDisplaySettings(NULL,0);
                return FALSE;
            }
        }
    };

}
}
