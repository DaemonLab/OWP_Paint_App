#include "painter.h"

Painter::Painter(HWND hwnd)
{
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

    // Create a Pencil tool as default tool
    currentTool = new PencilTool();
}

void Painter::startPaint(int x, int y)
{
    currentTool->startPaint(x, y, hdcMem, rc);
}

void Painter::continuePaint(int x, int y)
{
    currentTool->continuePaint(x, y, hdcMem, rc);
}

void Painter::endPaint()
{
    currentTool->endPaint();
}

void Painter::handleKeyPress(WPARAM key)
{
    switch(key)
    {
    case '1':   // Pencil
        delete currentTool;
        currentTool = new PencilTool();
        break;
    case '2':   // Rectangle
        delete currentTool;
        currentTool = new RectangleTool(false);
        break;
    case '3':   // Ellipse
        delete currentTool;
        currentTool = new EllipseTool(false);
        break;
    case '4':   // Filled Rectangle
        delete currentTool;
        currentTool = new RectangleTool(true);
        break;
    case '5':   // Filled Ellipse
        delete currentTool;
        currentTool = new EllipseTool(true);
    case '6':   // Change Pen Color
    case '7':   // Change Brush Color
    default:
        break;
    }
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

void PencilTool::startPaint(int x, int y, HDC hdc, RECT rc)
{
    SetPixel(hdc, x, y, RGB(255, 255, 255));     // Set the pixel white
}

void PencilTool::continuePaint(int x, int y, HDC hdc, RECT rc)
{
    SetPixel(hdc, x, y, RGB(255, 255, 255));     // Set the pixel white
}

void PencilTool::endPaint()
{
    // Do nothing
}

RectangleTool::RectangleTool(bool filled)
{
    this->filled = filled;
}

void RectangleTool::startPaint(int x, int y, HDC hdc, RECT rc)
{
    // Create an image of start time
    hdcStart = CreateCompatibleDC(hdc);
    bitmapStart = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdcStart, bitmapStart);
    BitBlt(hdcStart, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdc, 0, 0, SRCCOPY);
    startx = x;
    starty = y;
}

void RectangleTool::continuePaint(int x, int y, HDC hdc, RECT rc)
{
    // Rewrite the start state
    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcStart, 0, 0, SRCCOPY);
    HBRUSH prevBrush;
    // If the ellipse is not filled then replace the current brush with a
    // NULL_BRUSH and store the current brush
    if (!filled)
        prevBrush = (HBRUSH) SelectObject(hdc, GetStockObject(NULL_BRUSH));
    // draw a rectangle on top of that
    Rectangle(hdc, startx, starty, x, y);
    // Restore the original brush if required
    if (!filled)
        SelectObject(hdc, prevBrush);
}

void RectangleTool::endPaint()
{
    // Finish up. DOn't need the image of start anymore
    DeleteDC(hdcStart);
    DeleteObject(bitmapStart);
}

EllipseTool::EllipseTool(bool filled)
{
    this->filled = filled;
}

void EllipseTool::startPaint(int x, int y, HDC hdc, RECT rc)
{
    // Create an image of start time
    hdcStart = CreateCompatibleDC(hdc);
    bitmapStart = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    SelectObject(hdcStart, bitmapStart);
    BitBlt(hdcStart, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdc, 0, 0, SRCCOPY);
    startx = x;
    starty = y;
}

void EllipseTool::continuePaint(int x, int y, HDC hdc, RECT rc)
{
    // Rewrite the start state
    BitBlt(hdc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcStart, 0, 0, SRCCOPY);
    HBRUSH prevBrush;
    // If the ellipse is not filled then replace the current brush with a
    // NULL_BRUSH and store the current brush
    if (!filled)
        prevBrush = (HBRUSH) SelectObject(hdc, GetStockObject(NULL_BRUSH));
    // draw a rectangle on top of that
    Ellipse(hdc, startx, starty, x, y);
    // Restore the original brush if required
    if (!filled)
        SelectObject(hdc, prevBrush);
}

void EllipseTool::endPaint()
{
    // Finish up. Don't need the image of start anymore
    DeleteDC(hdcStart);
    DeleteObject(bitmapStart);
}
