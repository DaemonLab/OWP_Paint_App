#include "painter.h"

#include <tchar.h>
#include "utilities.h"

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
    // Release the DC got via GetDC
    ReleaseDC(hwnd, hdc);
    // Keep the paper in the paper holder
    SelectObject(hdcMem, bitmapMem);
    // First Clear the screen white
    SelectObject(hdcMem, GetStockObject(WHITE_BRUSH));
    SelectObject(hdcMem, GetStockObject(WHITE_PEN));
    Rectangle(hdcMem, 0, 0, rc.right, rc.bottom);
    // Select the brush and pen that will be kept on the holder
    // These will be used for drawing shapes.
    // We are using DC_PEN and DC_BRUSH so we can change their color when required.
    SelectObject(hdcMem, GetStockObject(DC_PEN));
    SelectObject(hdcMem, GetStockObject(DC_BRUSH));

    SetDCBrushColor(hdcMem, RGB(255, 255, 255));
    SetDCPenColor(hdcMem, RGB(0, 0, 0));

    // Create a Pencil tool as default tool and set current fileName to null
    currentTool = new PencilTool();
    curFileName[0] = '\0';
    changed = false;        // No edits yet
}

void Painter::startPaint(int x, int y)
{
    changed = true;
    currentTool->startPaint(x, y, hdcMem, rc);
}

void Painter::continuePaint(int x, int y)
{
    changed = true;
    currentTool->continuePaint(x, y, hdcMem, rc);
}

void Painter::endPaint()
{
    changed = true;
    currentTool->endPaint();
}

void Painter::handleKeyPress(HWND hwnd, WPARAM key)
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
        break;
    case '6':   // Change Pen Color
        COLORREF penClr;
        if (runChooseColorDialog(hwnd, &penClr))
        {
            SetDCPenColor(hdcMem, penClr);
        }
        break;
    case '7':   // Change Brush Color
        COLORREF brushClr;
        if (runChooseColorDialog(hwnd, &brushClr))
        {
            SetDCBrushColor(hdcMem, brushClr);
        }
    default:
        break;
    }
}

void Painter::draw(HWND hwnd)
{
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

Painter::~Painter()
{
    // Clear memory on destruction
    DeleteDC(hdcMem);
    DeleteObject(bitmapMem);

    delete currentTool;
}

void Painter::load_image(HBITMAP bitmap)
{
    // Create a new DC and BitBlt the image onto our hdcMem
    HDC hdcExtra = CreateCompatibleDC(hdcMem);
    SelectObject(hdcExtra, bitmap);
    BitBlt(hdcMem, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcExtra, 0, 0, SRCCOPY);
    DeleteDC(hdcExtra);
    changed = false;    // New image
}

void Painter::copyScreenToImage(HDC hdcTarget, CHAR* fileName)
{
    // Simply BitBlt the image onto the required target DC
    BitBlt(hdcTarget, 0, 0, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);
    changed = false;    // Saved the changes
    strcpy(curFileName, fileName);  // Copy the file name
}

int Painter::askForSaveIfReq(HWND hwnd)
{
    int res = IDNO;
    if(changed)
        res = MessageBox(hwnd, _T("Do you want to save the changes first?"), _T("Save changes?"), MB_YESNOCANCEL);
    return res;
}

bool Painter::hasFileName()
{
    return curFileName[0]!='\0';        // If it has been stored
}

void Painter::clearScreen()
{
    // Color the screen white and restore brush and pen
    HBRUSH oldBrush = (HBRUSH) SelectObject(hdcMem, GetStockObject(WHITE_BRUSH));
    HPEN oldPen = (HPEN) SelectObject(hdcMem, GetStockObject(WHITE_PEN));
    Rectangle(hdcMem, 0, 0, rc.right, rc.bottom);
    SelectObject(hdcMem, oldBrush);
    SelectObject(hdcMem, oldPen);
}

void Painter::getCurFileName(CHAR* fileName)
{
    // Copy the file name
    strcpy(fileName, curFileName);
}

void PencilTool::startPaint(int x, int y, HDC hdc, RECT rc)
{
    lastx = x;
    lasty = y;
    SetPixel(hdc, x, y, GetDCPenColor(hdc));     // Set the pixel with pen color
}

void PencilTool::continuePaint(int x, int y, HDC hdc, RECT rc)
{
    POINT p;
    MoveToEx(hdc, lastx, lasty, &p);
    LineTo(hdc, x, y);
    lastx = x;
    lasty = y;
    //SetPixel(hdc, x, y, GetDCPenColor(hdc));     // Set the pixel with pen color
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
