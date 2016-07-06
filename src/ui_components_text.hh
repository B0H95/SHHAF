#pragma once

#include <string>

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class Text
	    {
	    public:
		bool Init();
		void Deinit();
		void Draw();
		void SetPosition(int nx, int ny);
		void SetFontCharSize(int w, int h);
		void SetText(std::string text);
		void Highlight(bool light);

	    private:
		int x;
		int y;
		int fontw;
		int fonth;
		std::string str;
		bool highlighted;
	    };
	}
    }
}
