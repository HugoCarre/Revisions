#include "Settings.h"

int Settings::_screenHeight = 600;
int Settings::_screenWidth = 600;

int Settings::GetScreenHeight() {
	return _screenHeight;
}

int Settings::GetScreenWidth() {
	return _screenWidth;
}

void Settings::SetScreenHeight(int height) {
	_screenHeight = height;
}

void Settings::SetScreenWidth(int width) {
	_screenWidth = width;
}