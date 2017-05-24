#ifndef __MY_UTILITIES_H__
#define __MY_UTILITIES_H__

#include <windows.h>

// IDs for Menu items
#define IDM_FILE_NEW     1001
#define IDM_FILE_OPEN    1002
#define IDM_FILE_SAVE    1003
#define IDM_FILE_SAVE_AS 1004
#define IDM_FILE_QUIT    1005
#define IDM_HELP_ABOUT   2001
#define IDM_HELP_CTRLS   2002

// Function to add to windows
void addMenuBar (HWND parentWindow);
void addToolBar (HWND parentWindow);
// Creates shortcut table for Ctrl+ keys
HACCEL createShortcutsForMenus();

// Store the full file path in the char* or COLORREF variable
// false is returned if user cancelled the dialog
bool runOpenFileDialog(HWND hwnd, CHAR* openFilePath);
bool runSaveFileDialog(HWND hwnd, CHAR* saveFilePath);
bool runChooseColorDialog(HWND hwnd, COLORREF* colorChosen);

// A utility to save the HBITMAP to a .bmp file
void saveBitmapToFile(HBITMAP bitmap, CHAR* filePath, HWND hwnd);

#endif // __MY_UTILITIES_H__
