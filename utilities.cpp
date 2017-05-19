#include "utilities.h"

#include <commctrl.h>

void addMenuBar (HWND parentWindow)
{
    // Create handles/pointers to menubar and menu
    HMENU hMenubar;
    HMENU hFileMenu;

    hMenubar = CreateMenu();        // Create an object of the Menu class to act as the parent menubar
    hFileMenu = CreateMenu();           // Create an object of the menu to hold all the options for File menu

    // Use the API to add menu items with some text, some ID to the hFileMenu handle
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);              // A horizontal separator
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    // Append the File menu to menubar which will POPUP
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hFileMenu, L"&File");

    HMENU hHelpMenu;            // Create another menu
    hHelpMenu = CreateMenu();
    // Use the API to add menu items with some text, some ID to the hHelpMenu handle
    AppendMenuW(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, L"&About");
    // Append the Help menu to menubar which will POPUP
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hHelpMenu, L"&Help");

    // Set the menu for the desired window
    SetMenu(parentWindow, hMenubar);
}

void addToolBar (HWND parentWindow)
{
    // Create the toolbar from standard toolbar class
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
                                      WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
                                      parentWindow, NULL, GetModuleHandle(NULL), NULL);
    if (hWndToolbar == NULL)    // CreationFailed! *Rare*
        return;

    // Add standard windows icons to toolbar
    TBADDBITMAP tbab;
    tbab.hInst = HINST_COMMCTRL;        // COMMon ConTRoLs group
    tbab.nID = IDB_STD_SMALL_COLOR;     // Small and Colored icons
    SendMessage(hWndToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);

    // Create standard buttons for toolbar. Also we are using the same IDM_* IDs that we used for menubar
    TBBUTTON tbButtons[3];
    ZeroMemory(tbButtons, sizeof(tbButtons));   // Clear the memory of the array

    tbButtons[0].iBitmap = STD_FILENEW;       // Standard file->new icon
    tbButtons[0].fsState = TBSTATE_ENABLED;   // Button is enabled -- can be clicked
    tbButtons[0].fsStyle = TBSTYLE_BUTTON;    // Should look like a button
    tbButtons[0].idCommand = IDM_FILE_NEW;    // Our ID, we will receive this in WindowProcedure

    tbButtons[1].iBitmap = STD_FILEOPEN;
    tbButtons[1].fsState = TBSTATE_ENABLED;
    tbButtons[1].fsStyle = TBSTYLE_BUTTON;
    tbButtons[1].idCommand = IDM_FILE_OPEN;

    tbButtons[2].iBitmap = STD_FILESAVE;
    tbButtons[2].fsState = TBSTATE_ENABLED;
    tbButtons[2].fsStyle = TBSTYLE_BUTTON;
    tbButtons[2].idCommand = IDM_FILE_SAVE;

    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);     //Tell the toolbar about button size
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)3, (LPARAM)&tbButtons);     //Add 3 buttons to toolbar

    // Resize the toolbar, and then show it.
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(hWndToolbar,  TRUE);
}
