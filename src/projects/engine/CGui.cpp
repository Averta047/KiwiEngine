#include "CGui.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

CGui::CGui()
{
	m_hWnd = 0;
	m_pDevice = 0;
	m_pDeviceContext = 0;
}

CGui::~CGui()
{

}

bool CGui::Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    if (!hWnd)
        return false;

    if (!pDevice)
        return false;

    if (!pDeviceContext)
        return false;

    m_hWnd = hWnd;
    m_pDevice = pDevice;
    m_pDeviceContext = pDeviceContext;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = nullptr;               // Disable INI File  
    GImGui->NavDisableHighlight = true;     // Disable Highlighting

    if (!GImGui->NavDisableHighlight)
    {
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    }

    ImGui::StyleColorsDark();

    style.WindowRounding    = 5;
    style.ChildRounding     = 4;
    style.FrameRounding     = 3;
    style.GrabRounding      = 2;
    style.PopupRounding     = 2;
    style.TabRounding       = 2;
    style.ScrollbarRounding = 1;

    style.ButtonTextAlign   = { 0.5f, 0.5f };
    style.WindowTitleAlign  = { 0.5f, 0.5f };
    style.FramePadding      = { 8.0f, 8.0f };
    style.WindowPadding     = { 10.0f, 10.0f };
    style.ItemSpacing       = style.WindowPadding;
    style.ItemInnerSpacing  = { style.WindowPadding.x, 6 };

    style.WindowBorderSize  = 1;
    style.FrameBorderSize   = 1;

    style.ScrollbarSize     = 12.f;
    style.GrabMinSize       = 12.f;

    if (!ImGui_ImplWin32_Init(hWnd))
    {
        return false;
    }

    if (!ImGui_ImplDX11_Init(pDevice, pDeviceContext))
    {
        return false;
    }

    return true;
}

void CGui::Shutdown()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void CGui::Render()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        // Main Debug
        ImGui::Begin("Debug");
        {
            ImGui::Text("FPS: %.1f", io.Framerate);
            ImGui::SeparatorText("Cube");
            ImGui::SliderFloat("Rotation X", &Cube_Rotation_X, 0, 5);
            ImGui::SliderFloat("Rotation Y", &Cube_Rotation_Y, 0, 5);
            ImGui::SliderFloat("Rotation Z", &Cube_Rotation_Z, 0, 5);
        }
        ImGui::EndChild();
    }
    ImGui::Render();

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool CGui::MessageHandler(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    return ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);
}