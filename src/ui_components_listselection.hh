#pragma once

#include "ui_components_text.hh"

#include <string>
#include <vector>

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class ListSelection
	    {
	    public:
		bool Init();
		void Deinit();
		void ProcessInputs();
		void Draw();
		void AddMenuOption(std::string name, void (*callback)(void* data), void* extra);
		void SetFocus(bool f);
		void ClearList();
		void SetPosition(int nx, int ny);
		void SetFontSize(int w, int h);

	    private:
		SHH::UI::Components::Text textdrawer;
		int x;
		int y;
		int fontw;
		int fonth;
		bool focus;
		unsigned int listsize;
		unsigned int selection;
		std::vector<std::string> menuItems;
		std::vector<void(*)(void* data)> menuActions;
		std::vector<void*> menuActionExtra;
	    };
	}
    }
}
