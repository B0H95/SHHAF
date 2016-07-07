#pragma once

#include <string>

#include "ui_components_listselection.hh"
#include "ui_components_text.hh"
#include "ui_components_textinput.hh"

namespace SHH
{
    namespace UI
    {
	namespace Components
	{
	    class MenuOverlay
	    {
	    public:
		bool Init();
		void Deinit();
		void ProcessInputs();
		void Draw();
		bool Visible();

	    private:
		bool visible;
		bool joingroup;
		bool hostgroup;
		unsigned int joingroupselection;
		unsigned int hostgroupselection;
		std::string serveraddress[2];
		std::string serverconfig[2];
		SHH::UI::Components::ListSelection menulist;
		SHH::UI::Components::ListSelection joinmenulist;
		SHH::UI::Components::ListSelection hostmenulist;
		SHH::UI::Components::Text titletext;
		SHH::UI::Components::TextInput serverip, serverport, mapname;
	    };
	}
    }
}
