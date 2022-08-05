//
// Created by deny on 7/23/22.
//

#include "tbGameEngine.h"
#include <iostream>

tbGameEngine::tbGameEngine() {
    m_screenHeight = 30;
    m_screenWidth = 80;
    m_screenBuf = nullptr;
    m_elapsedTime = 0.f;
}

void tbGameEngine::Start() {
    //clear();
    Fill(0,0,m_screenWidth, m_screenHeight,SOLID,COLOR_GREEN, COLOR_BLACK);
    PlotLine(10,3,13,30,SOLID,COLOR_BLACK,COLOR_BLACK);
    std::thread t(&tbGameEngine::ThreadFunc, this);
    //wait for thread
    t.join();
}

void tbGameEngine::ThreadFunc() {
    while(1)
    {
        if(!OnUserUpdate(m_elapsedTime))
        {
            std::scoped_lock<std::mutex> lock(mutex);

        }
        OutputToTerminal();
        move(0,0);
        getch();
    }
}

tbGameEngine::~tbGameEngine() {
    endwin();
}

bool tbGameEngine::ConstructConsole(int width, int height) {
    std::string resizeCommand{"resize -s "};
    resizeCommand += std::to_string(height) + " " +std::to_string(width);
    system(resizeCommand.c_str());

    initscr();
    setlocale(LC_ALL, "");
    curs_set(0);
    start_color();
    noecho();

    m_screenWidth = width;
    m_screenHeight = height;
    m_screenBuf = new CHAR_INFO[height*width];
    std::memset(m_screenBuf,0,sizeof(cchar_t)*height*width);
    return true;
}

void tbGameEngine::Clip(int& x, int& y) {
    if(x > m_screenWidth)
        x = m_screenWidth;
    else if(x < 0)
        x = 0;
    if(y > m_screenHeight)
        y = m_screenHeight;
    else if(y < 0)
        y = 0;
}

void tbGameEngine::Fill(int x1, int y1, int x2, int y2, wchar_t ch, short frg_col,short bcg_col) {
    Clip(x1,y1);
    Clip(x2,y2);
    for (int x = x1; x < x2; ++x) {
        //frg_col = x % 8;
        for (int y = y1; y < y2 ; ++y) {
            PlotSingleChar(x,y,ch,frg_col,bcg_col);
        }

    }
}

void tbGameEngine::OutputToTerminal() {
    start_color();
    for (int r = 0; r < m_screenHeight; ++r) {
        for (int c = 0; c < m_screenWidth; ++c) {
            init_pair(m_screenBuf[r*m_screenWidth+c].Color.foreground+2, m_screenBuf[r*m_screenWidth+c].Color.foreground,
                      m_screenBuf[r*m_screenWidth+c].Color.background);
            attrset(COLOR_PAIR(m_screenBuf[r*m_screenWidth+c].Color.foreground+2));
            add_wch(&m_screenBuf[r*m_screenWidth+c].Char);

            //init_pair(1,COLOR_WHITE,-1);
            //attrset(COLOR_PAIR(1));

        }
    }
    //move(0,0);
    refresh();
}

void tbGameEngine::PlotSingleChar(int x, int y, wchar_t ch, short frg_col, short bcg_col) {
    if(x >= 0 && x <= m_screenWidth && y >= 0 && y <= m_screenHeight)
    {
        m_screenBuf[y*m_screenWidth+x].Char.chars[0] = ch;
        m_screenBuf[y*m_screenWidth+x].Char.attr = A_NORMAL;

        m_screenBuf[y*m_screenWidth+x].Color.foreground = frg_col;
        m_screenBuf[y*m_screenWidth+x].Color.background = bcg_col;
    }
}

int tbGameEngine::ScreenWidth() const{
    return m_screenWidth;
}

int tbGameEngine::ScreenHeight() const{
    return m_screenHeight;
}

void tbGameEngine::PlotLine(int x1, int y1, int x2, int y2, wchar_t ch, short frg_col, short bcg_col) {
/*    Clip(x1,y1);
    Clip(x2,y2);
    int dx = x2 - x1;
    int dy = y2 - y1;
    int D = 2*dy - dx;
    int y = y1;
    int dx1 = abs(dx);
    int dy1 = abs(dy);
    for(int x = x1; x < x2; x++)
    {
        PlotSingleChar(x,y,ch,frg_col,bcg_col);
        //screen_buffer[y*m_screenWidth+x].Char.chars[0]=ch;
        if (D > 0)
        {
            y = y + 1;
            D = D - 2*dx;
        }
        D = D + 2*dy;
    }*/
    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
    dx = x2 - x1; dy = y2 - y1;
    dx1 = abs(dx); dy1 = abs(dy);
    px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
    if (dy1 <= dx1)
    {
        if (dx >= 0)
        { x = x1; y = y1; xe = x2; }
        else
        { x = x2; y = y2; xe = x1;}

        PlotSingleChar(x, y, ch, frg_col,bcg_col);

        for (i = 0; x<xe; i++)
        {
            x = x + 1;
            if (px<0)
                px = px + 2 * dy1;
            else
            {
                if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
                px = px + 2 * (dy1 - dx1);
            }
            PlotSingleChar(x, y, ch, frg_col, bcg_col);
        }
    }
    else
    {
        if (dy >= 0)
        { x = x1; y = y1; ye = y2; }
        else
        { x = x2; y = y2; ye = y1; }

        PlotSingleChar(x, y, ch, frg_col, bcg_col);

        for (i = 0; y<ye; i++)
        {
            y = y + 1;
            if (py <= 0)
                py = py + 2 * dx1;
            else
            {
                if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
                py = py + 2 * (dx1 - dy1);
            }
            PlotSingleChar(x, y, ch, frg_col, bcg_col);
        }
    }
}
