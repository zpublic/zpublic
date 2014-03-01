#pragma once

// 123
#define STARTUP_WINDOW GWIN_ID_LOGIN

// <GENERATE id="enum">
// Auto Generated section, do not edit !!
    #define GWIN_ID_SUCCESS_EXIT 60000
    #define GWIN_ID_EXIT 60001
    #define GWIN_ID_CREATE_ROLE 61000
    #define GWIN_ID_LOGIN 61001
    #define GWIN_ID_MAIN 61002
    #define GWIN_ID_REGISTER 61003
    #define GWIN_ID_TRAY_MENU 61004
// </GENERATE>


class GameWindowControl
{
    public:
        static UINT_PTR start(const int&);
        inline static UINT_PTR start(){return start(STARTUP_WINDOW);}
        static void* create_dialog(const int&);

    private:
        GameWindowControl();

    // <GENERATE id="property">
    // Auto Generated section, do not edit !!
    public:
        inline static class GameCreateRoleDlg* get_create_role_dialog();
        static void destroy_create_role_dialog();
        inline static class GameLoginDlg* get_login_dialog();
        static void destroy_login_dialog();
        inline static class GameMainDlg* get_main_dialog();
        static void destroy_main_dialog();
        inline static class GameRegisterDlg* get_register_dialog();
        static void destroy_register_dialog();
        inline static class TrayMenuDlg* get_tray_menu_dialog();
        static void destroy_tray_menu_dialog();

    private:
        static class GameCreateRoleDlg* m_dlg_create_role;
        static class GameLoginDlg* m_dlg_login;
        static class GameMainDlg* m_dlg_main;
        static class GameRegisterDlg* m_dlg_register;
        static class TrayMenuDlg* m_dlg_tray_menu;
    // </GENERATE>
};

