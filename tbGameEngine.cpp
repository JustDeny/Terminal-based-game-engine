//
// Created by deny on 7/23/22.
//

#include "tbGameEngine.h"
#include <iostream>

tbGameEngine::tbGameEngine() {
    m_screenHeight = 30;
    m_screenWidth = 80;
    screen_buffer = nullptr;
    m_elapsedTime = 0.f;
}

void tbGameEngine::Start() {
    //clear();
    Fill(0,0,m_screenWidth, m_screenHeight,'A',COLOR_GREEN, COLOR_BLACK);
    std::thread t(&tbGameEngine::ThreadFunc, this);
    //wait for thread
    t.join();
}

void tbGameEngine::ThreadFunc() {
    while(1)
    {
        OutputToTerminal();
    }
    getch();
}

tbGameEngine::~tbGameEngine() {
    endwin();
}

bool tbGameEngine::ConstructConsole(int width, int height) {
    initscr();
    setlocale(LC_ALL, "");
    curs_set(0);
    start_color();

    std::string resizeCommand{"resize -s "};
    resizeCommand += std::to_string(height) + " " +std::to_string(width);
    system(resizeCommand.c_str());

    m_screenWidth = width;
    m_screenHeight = height;
    screen_buffer = new CHAR_INFO[height*width];
    std::memset(screen_buffer,0,sizeof(CHAR_INFO)*height*width);
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

void tbGameEngine::Fill(int x1, int y1, int x2, int y2, char16_t ch, short frg_col,short bcg_col) {
    Clip(x1,y1);
    Clip(x2,y2);
    for (int x = x1; x < x2; ++x) {
        for (int y = y1; y < y2 ; ++y) {
            PlotSingleChar(x,y,ch,frg_col,bcg_col);
        }
    }
}

void tbGameEngine::OutputToTerminal() {
    for (int r = 0; r < m_screenHeight; ++r) {
        for (int c = 0; c < m_screenWidth; ++c) {
            init_pair(1, screen_buffer[r*m_screenWidth+c].Color.foreground,
                      screen_buffer[r*m_screenWidth+c].Color.background);
            attrset(COLOR_PAIR(1));
            addch(screen_buffer[r*m_screenWidth+c].Char.unicodeChar);
        }
    }
    move(0,0);
    refresh();
}

void tbGameEngine::PlotSingleChar(int x, int y, char16_t ch, short frg_col, short bcg_col) {
    if(x >= 0 && x <= m_screenWidth && y >= 0 && y <= m_screenHeight)
    {
        screen_buffer[y*m_screenWidth+x].Char.unicodeChar = ch;
        screen_buffer[y*m_screenWidth+x].Color.foreground = frg_col;
        screen_buffer[y*m_screenWidth+x].Color.background = bcg_col;
    }
}

int tbGameEngine::ScreenWidth() const{
    return m_screenWidth;
}

int tbGameEngine::ScreenHeight() const{
    return m_screenHeight;
}

void tbGameEngine::PlotLine(int x1, int y1, int x2, int y2,char16_t ch, short frg_col, short bcg_col) {
    Clip(x1,y1);
    Clip(x2,y2);
}
