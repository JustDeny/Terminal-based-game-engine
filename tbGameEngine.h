//
// Created by deny on 7/23/22.
//

#ifndef TBGAMEENGINE_H
#define TBGAMEENGINE_H
#include <ncurses.h>
#include <thread>
#include <cstring>
#include <mutex>

enum TYPE_SHAPE: wchar_t
{
    SOLID = L'\u2588',
};

struct CHAR_INFO
{
/*    union{
        wchar_t unicodeChar;
        char asciiChar;
    }Char;*/
    cchar_t Char;
    struct
    {
        short foreground;
        short background;
    }Color;
};

class tbGameEngine {
public:
    int ScreenWidth() const;
    int ScreenHeight() const;
    tbGameEngine();
    bool ConstructConsole(int width, int height);
    ~tbGameEngine();
    void Start();
    void Clip(int &x, int &y);
    void Fill(int x1, int y1, int x2, int y2, wchar_t ch, short frg_col,short bcg_col);
    void PlotSingleChar(int x, int y, wchar_t ch,short frg_col,short bcg_col);
    void PlotLine(int x1, int y1, int x2, int y2,wchar_t ch, short frg_col,short bcg_col);
    void PlotCircle();
    void OutputToTerminal();
    virtual bool OnUserCreate() = 0;
    virtual bool OnUserUpdate(float elapsedTime) = 0;
private:
    void ThreadFunc();
    int m_screenWidth;
    int m_screenHeight;
    CHAR_INFO *m_screenBuf;
    std::mutex mutex;
    float m_elapsedTime;
    bool m_performing;
};


#endif
