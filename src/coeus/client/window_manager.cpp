#include "stdafx.h"
#include "window_manager.h"
#include "game_create_role_dlg.h"
#include "game_login_dlg.h"
#include "game_main_dlg.h"
#include "game_register_dlg.h"
#include "game_tray_menu_dlg.h"

GameCreateRoleDlg* GameWindowControl::m_dlg_create_role = NULL;
GameLoginDlg* GameWindowControl::m_dlg_login = NULL;
GameMainDlg* GameWindowControl::m_dlg_main = NULL;
GameRegisterDlg* GameWindowControl::m_dlg_register = NULL;
TrayMenuDlg* GameWindowControl::m_dlg_tray_menu = NULL;

void* GameWindowControl::create_dialog(const int &type)
{
    switch(type){
    case GWIN_ID_CREATE_ROLE:
        return (void*)get_create_role_dialog();
    case GWIN_ID_LOGIN:
        return (void*)get_login_dialog();
    case GWIN_ID_MAIN:
        return (void*)get_main_dialog();
    case GWIN_ID_REGISTER:
        return (void*)get_register_dialog();
    case GWIN_ID_TRAY_MENU:
        return (void*)get_tray_menu_dialog();
    }
    return NULL;
}

UINT_PTR GameWindowControl::start(const int &type)
{
    switch(type){
    case GWIN_ID_CREATE_ROLE:
        return get_create_role_dialog()->DoModal();
    case GWIN_ID_LOGIN:
        return get_login_dialog()->DoModal();
    case GWIN_ID_MAIN:
        return get_main_dialog()->DoModal();
    case GWIN_ID_REGISTER:
        return get_register_dialog()->DoModal();
    case GWIN_ID_TRAY_MENU:
        return get_tray_menu_dialog()->DoModal();
    }
    return GWIN_ID_EXIT;
}

GameCreateRoleDlg* GameWindowControl::get_create_role_dialog(){
    if(m_dlg_create_role == NULL){
        m_dlg_create_role = new GameCreateRoleDlg();
    }
    return m_dlg_create_role;
}

void GameWindowControl::destroy_create_role_dialog(){
    if(m_dlg_create_role != NULL){
        delete m_dlg_create_role;
        m_dlg_create_role = NULL;
    }
}

GameLoginDlg* GameWindowControl::get_login_dialog(){
    if(m_dlg_login == NULL){
        m_dlg_login = new GameLoginDlg();
    }
    return m_dlg_login;
}

void GameWindowControl::destroy_login_dialog(){
    if(m_dlg_login != NULL){
        delete m_dlg_login;
        m_dlg_login = NULL;
    }
}

GameMainDlg* GameWindowControl::get_main_dialog(){
    if(m_dlg_main == NULL){
        m_dlg_main = new GameMainDlg();
    }
    return m_dlg_main;
}

void GameWindowControl::destroy_main_dialog(){
    if(m_dlg_main != NULL){
        delete m_dlg_main;
        m_dlg_main = NULL;
    }
}

GameRegisterDlg* GameWindowControl::get_register_dialog(){
    if(m_dlg_register == NULL){
        m_dlg_register = new GameRegisterDlg();
    }
    return m_dlg_register;
}

void GameWindowControl::destroy_register_dialog(){
    if(m_dlg_register != NULL){
        delete m_dlg_register;
        m_dlg_register = NULL;
    }
}

TrayMenuDlg* GameWindowControl::get_tray_menu_dialog(){
    if(m_dlg_tray_menu == NULL){
        m_dlg_tray_menu = new TrayMenuDlg();
    }
    return m_dlg_tray_menu;
}

void GameWindowControl::destroy_tray_menu_dialog(){
    if(m_dlg_tray_menu != NULL){
        delete m_dlg_tray_menu;
        m_dlg_tray_menu = NULL;
    }
}

