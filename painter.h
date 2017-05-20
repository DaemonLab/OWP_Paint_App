#ifndef __PAINTER_H__
#define __PAINTER_H__

#include <windows.h>

// Our main painting class
class Painter
{
    // Buffer variables which act as canvas where we do the drawing
    // We later copy this to the main screen
    HDC hdcMem;
    HBITMAP bitmapMem;
public:
    Painter(HWND hwnd);
    void addPoint(int x, int y);
    void draw(HWND hwnd);
};


#endif // __PAINTER_H__
