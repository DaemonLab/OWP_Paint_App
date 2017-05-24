#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>

#include "utilities.h"      // Include our utility header
#include "painter.h"        // Include our painter module

/*  Declare procedures  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void savePainterToFile(HWND hwnd, CHAR* fileName);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("OWPApp");
HACCEL hAccel;  // Accelerator Table

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("OWP: A Paint App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    hAccel = createShortcutsForMenus();  // Create the accelerator table

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        // Try to dispatch the message via accelerator table
        // If the message is not for accelerators then dispatch normally
        if (!TranslateAccelerator(hwnd, hAccel, &messages))
        {
            /* Translate virtual-key messages into character messages */
            TranslateMessage(&messages);
            /* Send message to WindowProcedure */
            DispatchMessage(&messages);
        }
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

// Our global variables
Painter* global_painter = NULL;
bool mouse_drag = false;

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            addMenuBar (hwnd);           /* Add menubar to our window when it is created */
            addToolBar (hwnd);           /* Add toolbar to our window when it is created */
            global_painter = new Painter(hwnd);
            break;
        case WM_PAINT:
            // All the drawing on actual window must be done in WM_PAINT event only
            global_painter->draw(hwnd);
            break;
        case WM_LBUTTONDOWN:
            {
                mouse_drag = true;      // Set it to true so that later mouse move events are counted as drag
                // Get the mouse location from lParam
                int mousx = LOWORD(lParam);
                int mousy = HIWORD(lParam);
                // Add the point on our buffer canvas
                global_painter->startPaint(mousx, mousy);
                // Invalidate the window and force redraw so that all the changes are visible instantly
                // This will trigger WM_PAINT event
                InvalidateRect(hwnd, NULL, FALSE);
                UpdateWindow(hwnd);
                break;
            }
        case WM_LBUTTONUP:
            if (mouse_drag)
            {
                global_painter->endPaint();
                mouse_drag = false;     // Mouse drag has stopped
            }
            break;
        case WM_MOUSEMOVE:
            if (mouse_drag)
            {
                // Add more points if mouse is being dragged
                int mousx = LOWORD(lParam);
                int mousy = HIWORD(lParam);
                global_painter->continuePaint(mousx, mousy);
                InvalidateRect(hwnd, NULL, FALSE);
                UpdateWindow(hwnd);
            }
            break;
        case WM_KEYDOWN:
            global_painter->handleKeyPress(hwnd, wParam);
            break;
        case WM_COMMAND:               /* Handle all the commands */
            switch(LOWORD(wParam))       /* switch to ID of the menu that sent the command */
            {
                case IDM_FILE_NEW:
                    switch(global_painter->askForSaveIfReq(hwnd))
                    {
                    case IDYES:
                        SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0);
                        break;
                    case IDNO:
                        break;
                    case IDCANCEL:
                        return 0;       // User doesn't want new screen
                    }
                    global_painter->clearScreen();
                    InvalidateRect(hwnd, NULL, FALSE);
                    UpdateWindow(hwnd);
                    break;
                case IDM_FILE_OPEN:
                    switch(global_painter->askForSaveIfReq(hwnd))
                    {
                    case IDYES:
                        SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0);
                        break;
                    case IDNO:
                        break;
                    case IDCANCEL:
                        return 0;       // User doesn't want to open
                    }
                    CHAR openFilePath[MAX_PATH];
                    if (runOpenFileDialog(hwnd, openFilePath))
                    {
                        HBITMAP loaded_image = (HBITMAP) LoadImage (NULL, openFilePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                        global_painter->load_image(loaded_image);
                        DeleteObject(loaded_image);
                        InvalidateRect(hwnd, NULL, FALSE);
                        UpdateWindow(hwnd);
                    }
                    break;
                case IDM_FILE_SAVE:
                    if (!global_painter->hasFileName()) // First Time Save
                        SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE_AS, 0);
                    else
                    {
                        // Overwrite the existing file
                        CHAR saveFilePath[MAX_PATH];
                        global_painter->getCurFileName(saveFilePath);
                        savePainterToFile(hwnd, saveFilePath);
                    }
                    break;
                case IDM_FILE_SAVE_AS:
                    CHAR saveAsFilePath[MAX_PATH];
                    if (runSaveFileDialog(hwnd, saveAsFilePath))
                        savePainterToFile(hwnd, saveAsFilePath);
                    break;
                case IDM_FILE_QUIT:
                    /* Send a CLOSE signal to the window so that it may clear up everything and exit properly */
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                case IDM_HELP_ABOUT:
                    MessageBox(hwnd,          // The window over which messagebox is show
                               _T("Made under One Week Projects! \n"
                                  "Visit https://github.com/DaemonLab for more info"),// Text in the messagebox
                               _T("About"),       // Title of the message box
                               MB_OK | MB_ICONINFORMATION);   // The buttons and icon in messagebox
                    break;
                case IDM_HELP_CTRLS:
                    MessageBox(hwnd,          // The window over which messagebox is show
                               _T("Use the following buttons to change the drawing tools: \n"
                                  "1 -- pencil \n"
                                  "2 -- rectangle \n"
                                  "3 -- ellipse \n"
                                  "4 -- filled rectangle \n"
                                  "5 -- filled ellipse \n"
                                  "6 -- change pen color \n"
                                  "7 -- change brush color "),// Text in the messagebox
                               _T("Controls"),       // Title of the message box
                               MB_OK | MB_ICONINFORMATION);   // The buttons and icon in messagebox
                    break;
            }
            break;
        case WM_CLOSE:
            switch(global_painter->askForSaveIfReq(hwnd))
            {
            case IDYES:
                SendMessage(hwnd, WM_COMMAND, IDM_FILE_SAVE, 0);
                break;
            case IDNO:
                break;
            case IDCANCEL:
                return 0;       // User doesn't want to exit
            }
            return DefWindowProc (hwnd, message, wParam, lParam);
            break;
        case WM_DESTROY:
            // Destroy the accelerator table
            DestroyAcceleratorTable(hAccel);
            // Clear the memory for the allocated pointer
            if (global_painter)
                delete global_painter;
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

void savePainterToFile(HWND hwnd, CHAR* fileName)
{
    // Create a copy DC, make the painter paint the image on the copy
    // then save the copy bitmap. Finally release the memory
    HDC hdc = GetDC(hwnd);
    RECT rc;
    GetWindowRect(hwnd, &rc);
    HDC hdcTarget = CreateCompatibleDC(hdc);
    HBITMAP bitmapTarget = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    ReleaseDC(hwnd, hdc);
    SelectObject(hdcTarget, bitmapTarget);
    global_painter->copyScreenToImage(hdcTarget, fileName);
    saveBitmapToFile(bitmapTarget, fileName, hwnd);
    DeleteDC(hdcTarget);
    DeleteObject(bitmapTarget);
}
