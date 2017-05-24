#include "utilities.h"

#include <commctrl.h>
#include <windows.h>
#include <cstdio>

void addMenuBar (HWND parentWindow)
{
    // Create handles/pointers to menubar and menu
    HMENU hMenubar;
    HMENU hFileMenu;

    hMenubar = CreateMenu();        // Create an object of the Menu class to act as the parent menubar
    hFileMenu = CreateMenu();           // Create an object of the menu to hold all the options for File menu

    // Use the API to add menu items with some text, some ID to the hFileMenu handle
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_NEW,     L"&New  \tCtrl+N");
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_OPEN,    L"&Open \tCtrl+O");
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_SAVE,    L"&Save \tCtrl+S");
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_SAVE_AS, L"&Save As");
    AppendMenuW(hFileMenu, MF_SEPARATOR, 0, NULL);              // A horizontal separator
    AppendMenuW(hFileMenu, MF_STRING, IDM_FILE_QUIT,    L"&Quit \tCtrl+Q");

    // Append the File menu to menubar which will POPUP
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hFileMenu, L"&File");

    HMENU hHelpMenu;            // Create another menu
    hHelpMenu = CreateMenu();
    // Use the API to add menu items with some text, some ID to the hHelpMenu handle
    AppendMenuW(hHelpMenu, MF_STRING, IDM_HELP_ABOUT, L"&About");
    AppendMenuW(hHelpMenu, MF_STRING, IDM_HELP_CTRLS, L"&Controls");
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

// One function for both "Open" and "Save" commands:
BOOL runFileDialog (HWND hwnd, CHAR* pstrResultPath, bool saveDialog) {
    OPENFILENAME ofn = {0};
    BOOL bRetCode;

    ofn.lStructSize = sizeof (ofn);     // Its own size
    ofn.hwndOwner = hwnd;       // The window over which this dialog will be shown
    ofn.Flags = OFN_PATHMUSTEXIST;      // The selected folder path must exist, obviously!
    ofn.lpstrFile = pstrResultPath;     // Variable pointer to store the resultant path
    // Filters shown in the dialog bos
    ofn.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0";
    ofn.lpstrDefExt = "bmp";
    // maximum path size is given by MAX_PATH constant
    ofn.nMaxFile = MAX_PATH;
    pstrResultPath [0] = '\0';  // Initialise by NULL

    if (saveDialog)
    {
        // ask confirmation for overwrite if file exists while saving
        ofn.Flags |= OFN_OVERWRITEPROMPT;
        bRetCode = GetSaveFileName (&ofn);
    }
    else
    {
        // make sure the specified file name must exist when trying to open
        ofn.Flags |= OFN_FILEMUSTEXIST;
        bRetCode = GetOpenFileName (&ofn);
    }
    return bRetCode;
}

bool runOpenFileDialog(HWND hwnd, CHAR* openFilePath)
{
    return runFileDialog(hwnd, openFilePath, false);
}

bool runSaveFileDialog(HWND hwnd, CHAR* saveFilePath)
{
    return runFileDialog(hwnd, saveFilePath, true);
}

bool runChooseColorDialog(HWND hwnd, COLORREF* colorChosen)
{
    CHOOSECOLOR cc; // common dialog box structure
     // array of custom colors. this has to be static
     // as it will not be lost between calls to this function
     // since user can store custom colors
    static COLORREF acrCustClr[16];
    // Initialize CHOOSECOLOR
    ZeroMemory(&cc, sizeof(CHOOSECOLOR));
    cc.lStructSize = sizeof(CHOOSECOLOR);
    cc.hwndOwner = hwnd;        // Window over which it will be shown
    cc.lpCustColors = (LPDWORD) acrCustClr; // 16 custom colors
    COLORREF clr;
    cc.rgbResult = clr;     // Initial value
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;
    bool retVal = ChooseColor(&cc);
    *colorChosen = cc.rgbResult;    // Store the result in variable
    return retVal;
}

void saveBitmapToFile(HBITMAP bitmap, CHAR* filePath, HWND hwnd)
{
    RECT client;
    GetWindowRect(hwnd, &client);
    UINT uiBytesPerRow = 3 * client.right; // RGB takes 24 bits
    UINT uiRemainderForPadding;

    // Make the bytes per row to be multiple of DWORD size
    if ((uiRemainderForPadding = uiBytesPerRow % sizeof (DWORD)) > 0)
        uiBytesPerRow += (sizeof (DWORD) - uiRemainderForPadding);

    // Calculate and allocated total memory
    UINT uiBytesPerAllRows = uiBytesPerRow * client.bottom;
    PBYTE pDataBits;
    if ((pDataBits = new BYTE [uiBytesPerAllRows]) != NULL)
    {
        BITMAPINFOHEADER bmi = {0};
        BITMAPFILEHEADER bmf = {0};
        HDC hDC = GetDC (hwnd);

        // Prepare to get the data out of HBITMAP:
        bmi.biSize = sizeof (bmi);
        bmi.biPlanes = 1;
        bmi.biBitCount = 24;
        bmi.biHeight = client.bottom;
        bmi.biWidth = client.right;

        // Get it:
        GetDIBits (hDC, bitmap, 0, client.bottom,
            pDataBits, (BITMAPINFO*) &bmi, DIB_RGB_COLORS);

        ReleaseDC (hwnd, hDC);  //release the DC got via GetDC

        // Fill the file header:
        bmf.bfOffBits = sizeof (bmf) + sizeof (bmi);
        bmf.bfSize = bmf.bfOffBits + uiBytesPerAllRows;
        bmf.bfType = 0x4D42;

        // Write the actual file as per BMP file format
        FILE* pFile;
        if ((pFile = fopen (filePath, "wb")) != NULL)
        {
            fwrite (&bmf, sizeof (bmf), 1, pFile);
            fwrite (&bmi, sizeof (bmi), 1, pFile);
            fwrite (pDataBits, sizeof (BYTE), uiBytesPerAllRows, pFile);
            fclose (pFile);
        }
        delete [] pDataBits;
    }
}

HACCEL createShortcutsForMenus()
{
    // Create a table of what keypress will send what signal
    ACCEL allAccelerators[4];
    // FVIRTKEY implies that the keycode should be taken independently
    // that is, pressing 'N' does nothing without pressing Ctrl too (FCONTROL)
    allAccelerators[0].fVirt = FCONTROL | FVIRTKEY;      // Ctrl must be pressed
    allAccelerators[0].key = 'N';             // N key with Ctrl
    allAccelerators[0].cmd = IDM_FILE_NEW;    // Will send IDM_FILE_NEW
    allAccelerators[1].fVirt = FCONTROL | FVIRTKEY;
    allAccelerators[1].key = 'O';
    allAccelerators[1].cmd = IDM_FILE_OPEN;
    allAccelerators[2].fVirt = FCONTROL | FVIRTKEY;
    allAccelerators[2].key = 'S';
    allAccelerators[2].cmd = IDM_FILE_SAVE;
    allAccelerators[3].fVirt = FCONTROL | FVIRTKEY;
    allAccelerators[3].key = 'Q';
    allAccelerators[3].cmd = IDM_FILE_QUIT;

    return CreateAcceleratorTable(allAccelerators, 4);
}
