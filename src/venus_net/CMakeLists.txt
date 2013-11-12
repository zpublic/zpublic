#cmake file for project association
#author:>---AngryPowman
#created:>--2013/11/11

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(venus_net cxx)

SET(LIB_SRC_LIST 
./acceptor.cpp
./ascii.cpp
./error_code.cpp
./local_free_on_destruction.hpp
./name_value_collection.cpp
./object_pool.hpp
./server_logger.cpp
./socket.cpp
./tcp_client.cpp
./tcp_connection.cpp
./tcp_server.cpp
./protocol/10000_C2SLoginReq.pb.cc
./protocol/10001_S2CLoginRsp.pb.cc
./protocol/10002_C2SRegisterReq.pb.cc
./protocol/10003_S2CRegisterRsp.pb.cc
./protocol/15000_C2SGetPlayerProfileReq.pb.cc
./protocol/15001_S2CGetPlayerProfileRsp.pb.cc
./protocol/20000_C2SChatMessageReq.pb.cc
./protocol/20001_S2CChatMessageNotify.pb.cc
./protocol/21001_C2SRoomCreateReq.pb.cc
./protocol/21002_S2CRoomCreateRsp.pb.cc
./protocol/21003_C2SGetRoomListReq.pb.cc
./protocol/21004_S2CGetRoomListRsp.pb.cc
./protocol/22001_C2STransferAccountsReq.pb.cc
./protocol/22002_S2CTransferAccountsRsp.pb.cc
./protocol/99990_S2CError.pb.cc
./protocol/99991_S2CErrorEx.pb.cc
)

##输出为静态库
ADD_LIBRARY(libvenus_net ${LIB_SRC_LIST})