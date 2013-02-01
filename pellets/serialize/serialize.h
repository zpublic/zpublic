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
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#ifndef ZL_SERIALIZE_H
#define ZL_SERIALIZE_H

/*************************************************************************

序列化解释：
    序列化是将对象状态转换为可保持或传输的格式的过程。与序列化相对的是反序列化，它
将流转换为对象。这两个过程结合起来，可以轻松地存储和传输数据。

序列化的作用：
    1、以某种存储形式使自定义对象持久化；
    2、将对象从一个地方传递到另一个地方；
    3、使程序更具维护性。

序列化后的数据，可以是明文（参考xml,json）也可以是二进制的（参考protobuf）。
序列化后的数据，可以包含格式信息，也可以由外部格式定义来实现反序列化。

在zpublic里实现的序列化，使用二进制存储数据，并且使用外部格式定义来进行转换。
对象的存储使用到pellets/DataBox。

*************************************************************************/

#include "serialize_def.h"
#include "data_format.h"
#include "data.h"
#include "serialize_engine.h"

#endif