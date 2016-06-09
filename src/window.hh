#pragma once

#include <string>
#include <cstdint>

namespace SHH
{
    namespace Window
    {
	bool Init(int width, int height, std::string name);
	void Deinit();
	void ProcessEvents();
	int GetWindowWidth();
	int GetWindowHeight();
	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	void ClearScreen();
	void UpdateScreen();
	void DrawRectangle(int x1, int y1, int x2, int y2);
	void DrawFilledRectangle(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);
	void DrawPoint(int x, int y);
    }
}
