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
#ifndef _LTEST_SUITE_HPP_
#define _LTEST_SUITE_HPP_

///> ≤‚ ‘∞∏¿˝¿‡

namespace ltest
{
    typedef void (*TestFunc)();
    typedef struct tagSingleTest
    {
        tagSingleTest(const std::string& name, TestFunc func)
        {
            d_name = name;
            d_func = func;
        }
        std::string     d_name;
        TestFunc        d_func;
    }SingleTest;
    typedef std::list<SingleTest> TestList;
    typedef std::list<SingleTest>::const_iterator TestListIt;

    class TestSuite
    {
        friend class TestActor;
    public:
        TestSuite()
        {

        }
        virtual ~TestSuite()
        {

        }

        void Add(const std::string& name, TestFunc func)
        {
            m_testList.push_back(SingleTest(name, func));
        }

    private:
        TestList m_testList;
    };

}

#endif // _LTEST_SUITE_HPP_
