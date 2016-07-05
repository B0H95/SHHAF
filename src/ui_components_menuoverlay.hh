#pragma once

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
		unsigned int selection;
	    };
	}
    }
}
