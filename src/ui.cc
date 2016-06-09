#include "ui.hh"

#include "log.hh"
#include "window.hh"
#include "programcontroller.hh"

bool SHH::UI::Init()
{
    SHH::Log::Log("SHH::UI::Init(): Started.");
    SHH::Log::Log("SHH::UI::Init(): Ended successfully.");
    return true;
}

void SHH::UI::Deinit()
{
    SHH::Log::Log("SHH::UI::Deinit(): Started.");
    SHH::Log::Log("SHH::UI::Deinit(): Ended successfully.");
}

void SHH::UI::Draw()
{
    SHH::Window::SetColor(0x00, 0x00, 0x00, 0xFF);
    SHH::Window::ClearScreen();

    SHH::Window::SetColor(0x00, 0xFF, 0x00, 0xFF);

    int frameLoadBarWidth = int((float)SHH::Window::GetWindowWidth() * SHH::ProgramController::GetFrameLoad());
    SHH::Window::DrawFilledRectangle(0, 0, frameLoadBarWidth, 32);
    
    SHH::Window::UpdateScreen();
}

void SHH::UI::ProcessInputs()
{
    
}
