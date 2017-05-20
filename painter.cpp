#include "painter.h"

Painter::Painter(HWND hwnd)
{
    RECT rc;
    // Get Size of the window. Width of the window will be rc.right-rc.left
    // similarly height of the window will be rc.bottom-rc.top
    // The y coordinate increases downwards and x coordinate increases rightward
    // So the top left corner is (0,0) and bottom right corner is (xmax, ymax)
    GetWindowRect(hwnd, &rc);

    HDC hdc = GetDC(hwnd);  // Get the handle to Device Context of the window
    hdcMem = CreateCompatibleDC(hdc);   // Create a DC which resembles the actual screen
    // A bitmap is where actual drawing takes place. A DC is a paper holder while bitmap is
    // the paper on which drawing is done. We create a bitmap which resembles the paper on screen
    bitmapMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);

    // Keep the paper in the paper holder
    SelectObject(hdcMem, bitmapMem);
    // Select the brush and pen that will be kept on the holder
    // These will be used for drawing shapes. We are using predefined bursh and pen.
    SelectObject(hdcMem, GetStockObject(BLACK_BRUSH));
    SelectObject(hdcMem, GetStockObject(WHITE_PEN));
}

void Painter::addPoint(int x, int y)
{
    SetPixel(hdcMem, x, y, RGB(255, 255, 255));     // Set the pixel white
}

void Painter::draw(HWND hwnd)
{
    RECT rc;
    GetWindowRect(hwnd, &rc);   // Get the window size
    HDC     hdc;
    PAINTSTRUCT ps;         // Required by BeginPaint
    // We must do all the painting on actual device context only
    // between BeginPaint and EndPaint as they put in some restrictions
    // and some safe-gaurds like not drawing outside the permitted region and
    // saving the mouse pointer from being drawn upon
    hdc = BeginPaint(hwnd, &ps);    // Get the actual Device Context of the window
    // Copy our buffer canvas onto the real canvas
    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);
    EndPaint(hwnd, &ps);    // Signal end of painting
}
