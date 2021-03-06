#ifndef __PAINTER_H__
#define __PAINTER_H__

#include <windows.h>

// A base class to represent all the paint tools
class PaintTool
{
public:
    // Called for LBUTTONDOWN
    virtual void startPaint(int x, int y, HDC hdc, RECT rc) = 0;
    // Called while mouse is dragged
    virtual void continuePaint(int x, int y, HDC hdc, RECT rc) = 0;
    // Called when LBUTTONUP
    virtual void endPaint() = 0;
};

// Our main painting class
class Painter
{
private:
    // To store size of the window
    RECT rc;
    // Buffer variables which act as canvas where we do the drawing
    // We later copy this to the main screen
    HDC hdcMem;
    HBITMAP bitmapMem;

    PaintTool* currentTool; // Current drawing tool
    CHAR curFileName[MAX_PATH];
    bool changed;
public:
    Painter(HWND hwnd);
    ~Painter();

    void startPaint(int x, int y);
    void continuePaint(int x, int y);
    void endPaint();

    void handleKeyPress(HWND hwnd, WPARAM key);
    void draw(HWND hwnd);

    void getCurFileName(CHAR* fileName);

    // Display the HBITMAP on hdcMem
    void load_image(HBITMAP bitmap);
    // copy contents of hdcMem to the BITMAP
    void copyScreenToImage(HDC hdcTarget, CHAR* fileName);
    // ask the user to save changes if there are new changes
    int askForSaveIfReq(HWND hwnd);
    // Check if user has already saved this to a file
    bool hasFileName();
    // Clear the painting
    void clearScreen();
};

/* All the Paint Tools */

// Pencil
class PencilTool : public PaintTool
{
private:
    int lastx;
    int lasty;
public:
    virtual void startPaint(int x, int y, HDC hdc, RECT rc);
    virtual void continuePaint(int x, int y, HDC hdc, RECT rc);
    virtual void endPaint();
};

// Rectangle
class RectangleTool : public PaintTool
{
private:
    // Need these to store the state of screen before startPaint
    // So that mouse drag doesn't make more shapes
    HDC hdcStart;
    HBITMAP bitmapStart;
    int startx;
    int starty;
    bool filled;
public:
    RectangleTool(bool filled);
    virtual void startPaint(int x, int y, HDC hdc, RECT rc);
    virtual void continuePaint(int x, int y, HDC hdc, RECT rc);
    virtual void endPaint();
};

// Ellipse
class EllipseTool : public PaintTool
{
private:
    // Need these to store the state of screen before startPaint
    // So that mouse drag doesn't make more shapes
    HDC hdcStart;
    HBITMAP bitmapStart;
    int startx;
    int starty;
    bool filled;
public:
    EllipseTool(bool filled);
    virtual void startPaint(int x, int y, HDC hdc, RECT rc);
    virtual void continuePaint(int x, int y, HDC hdc, RECT rc);
    virtual void endPaint();
};

#endif // __PAINTER_H__
