#include <common.h>
#include <network_common.h>
#include <protobuf.h>
#include <packet.h>
#include "game_session_manager.h"
#include "game_session.h"
#include "room_manager.h"
#include "player.h"


void GameSession::room_create_handler(const NetworkMessage& message)
{
    Protocol::C2SRoomCreateReq request;
    PARSE_NETWORK_MESSAGE(message, request);

    debug_log("[Room Create] -> (RoomName='%s', Password='%s')", request.room_name().c_str(), request.password().c_str());

    Protocol::S2CRoomCreateRsp response;
    //验证房间名长度
    if ( request.room_name().length() < 24)
    {   
        if ( request.room_name().length() > 0 )
        {           
            //验证密码长度
            if ( request.password().length() <= 6 )
            {
                uint32 id = RoomManager::getInstance().addRoom(request.room_name(), request.password(), _player->guid());
                response.set_room_id(id);
                if (id != 0)
                {
                    response.set_room_create_result(true);
                    broadcast_room_add(id, request.room_name(), request.password().empty());
                }
                else
                {
                    response.set_room_create_result(false);
                    response.set_failed_reason("房间数量已满。");
                }
            }
            else
            {
                warning_log("Room password has reached the limits length.");
                response.set_room_create_result(false);
                response.set_failed_reason("房间密码过长。");
            }
        }
        else
        {
            warning_log("Room name can't be empty");
            response.set_room_create_result(false);
            response.set_failed_reason("房间名不可为空。");
        }
        
    }
    else
    {
        warning_log("Room name has reached the limits length.");
        response.set_room_create_result(false);
        response.set_failed_reason("房间名过长。");
    }
    send_message<Protocol::S2CRoomCreateRsp>(Opcodes::S2CRoomCreateRsp, response);
}

void GameSession::broadcast_room_add(uint32 id, const std::string& roomName, bool isPublic)
{
    Protocol::S2CNewRoomAddNotify response;
    response.set_room_id(id);
    response.set_room_name(roomName);
    response.set_public_(isPublic);
    GameSessionManager::getInstance().broadcast<Protocol::S2CNewRoomAddNotify>(Opcodes::S2CNewRoomAddRsp, response);
}

void GameSession::get_room_list_handler(const NetworkMessage& message)
{
    Protocol::S2CGetRoomListRsp response;
    adap_map<uint32, Room*> rooms;
    RoomManager::getInstance().getRooms(rooms);

    for (auto it = rooms.begin(); it != rooms.end(); ++it)
    {
        Protocol::S2CGetRoomListRsp::RoomInfo* roomInfo = response.add_room_list();
        roomInfo->set_room_id(it->second->getId());
        roomInfo->set_room_name(it->second->getRoomName());
        roomInfo->set_player_count(it->second->getPlayersCount());
    }
    debug_log("Send room list to client.");
    send_message<Protocol::S2CGetRoomListRsp>(Opcodes::S2CGetRoomListRsp ,response);
}

void GameSession::enter_room_handler(const NetworkMessage& message)
{
    Protocol::C2SEnterRoomReq request;
    PARSE_NETWORK_MESSAGE(message, request);
    debug_log("[Enter Room]: room_id = '%d'", request.room_id());

    Protocol::S2CEnterRoomRsp response;

    Room* room = RoomManager::getInstance().getRoom(request.room_id());
    if(room != NULL)
    {
        room->addMember(_player->guid());
        response.set_result(true);
    }
    else
    {
        warning_log("Can't find room");
        response.set_result(false);
    }
    send_message<Protocol::S2CEnterRoomRsp>(Opcodes::S2CEnterRoomRsp, response);
}