#pragma once

namespace SHH
{
    namespace Simulation
    {
	namespace InputMessages
	{
	    bool Init();
	    void Deinit();
	    void FlushMessages();
	    void FetchSimMessages();
	    void FetchCtrlMessages();
	    void DistributeMessages();
	}
    }
}
