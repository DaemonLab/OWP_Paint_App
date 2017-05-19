#ifndef __MY_UTILITIES_H__
#define __MY_UTILITIES_H__

#include <windows.h>

// IDs for Menu items
#define IDM_FILE_NEW   1001
#define IDM_FILE_OPEN  1002
#define IDM_FILE_SAVE  1003
#define IDM_FILE_QUIT  1004
#define IDM_HELP_ABOUT 2001

// Function to add to windows
void addMenuBar (HWND parentWindow);
void addToolbar (HWND parentWindow);

#endif // __MY_UTILITIES_H__
