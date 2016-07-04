#include "ui_components_performancemonitor.hh"

#include "window.hh"
#include "programcontroller.hh"

bool SHH::UI::Components::PerformanceMonitor::Init()
{
    return true;
}

void SHH::UI::Components::PerformanceMonitor::Deinit()
{
}

void SHH::UI::Components::PerformanceMonitor::ProcessInputs()
{
}

void SHH::UI::Components::PerformanceMonitor::Draw()
{
    SHH::Window::SetColor(0x00, 0xFF, 0x00, 0xFF);
    int frameLoadBarWidth = int((float)SHH::Window::GetWindowWidth() * SHH::ProgramController::GetFrameLoad());
    SHH::Window::DrawFilledRectangle(0, 0, frameLoadBarWidth, 32);
}
