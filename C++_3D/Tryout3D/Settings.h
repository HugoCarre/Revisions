#pragma once

class Settings
{
public:
    static int GetScreenWidth();
    static int GetScreenHeight();
    static void SetScreenWidth(int width);
    static void SetScreenHeight(int height);
private:
    static int _screenWidth;
    static int _screenHeight;        
};