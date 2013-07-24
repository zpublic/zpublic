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
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *                             pjj(trueme521@hotmail.com)                *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#ifndef _LTEST_ACTOR_HPP_
#define _LTEST_ACTOR_HPP_

///> ²âÊÔÖ´ÐÐÕß

namespace ltest
{
    class TestActor
    {
    public:
        TestActor()
        {
        }

        virtual ~TestActor()
        {
        }

        void Run(TestSuite& test)
        {
            for (TestListIt it = test.m_testList.begin();
                it != test.m_testList.end();
                ++it)
            {
                it->d_func();
            }
        }

    private:

    };

}

#endif // _LTEST_ACTOR_HPP_
