#pragma once

enum emUserMsg
{
    msg_login_result        = WM_USER + 2000,       ///> 登陆结果
    msg_register_result,                            ///> 注册结果
    msg_create_role_result,                         ///> 创号结果
    msg_get_rand_name_result,                       ///> 随机昵称结果
    msg_check_nickname_result,                      ///> 检查昵称可用结果
    msg_goto_world,                                 ///>
    msg_player_info,
    msg_traymessage,                                ///> 托盘消息
    msg_game_quit,                                  ///> 退出游戏
};
