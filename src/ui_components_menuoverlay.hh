#pragma once

#include "ui_components_listselection.hh"
#include "ui_components_text.hh"

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
		SHH::UI::Components::ListSelection menulist;
		SHH::UI::Components::Text titletext;
	    };
	}
    }
}
