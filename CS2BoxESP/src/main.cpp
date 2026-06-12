#pragma once
#include <stdint.h>
#include <math.h>
#include <windows.h>
#include <d3d11.h>
#include "MinHook/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "config.h"
#include "gui.h"
#include "overlay.h"
#include "Utils/Offsets.h"
#include "Utils/Types.h"
#include "Utils/Math.h"
#include "esp.h"
#include "features/aimbot.h"
#include "features/bhop.h"

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
HWND g_hWnd = nullptr;
bool g_ImGuiInitialized = false;
bool g_Unloading = false;
HANDLE g_hFOVThread = nullptr;

WNDPROC oWndProc = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT __stdcall hkWndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_KEYDOWN && wParam == VK_INSERT) {
        Config::bMenuOpen = !Config::bMenuOpen;
        return 1;
    }
    if (Config::bMenuOpen) {
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
        ImGuiIO& io = ImGui::GetIO();
        if ((uMsg >= WM_MOUSEFIRST && uMsg <= WM_MOUSELAST)) {
            if (io.WantCaptureMouse) return 1;
        }
        if ((uMsg >= WM_KEYFIRST && uMsg <= WM_KEYLAST)) {
            if (io.WantCaptureKeyboard) return 1;
        }
    }
    return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

typedef HRESULT(__stdcall* Present_t)(IDXGISwapChain*, UINT, UINT);
Present_t oPresent = nullptr;

typedef void(__stdcall* CreateMove_t)(uintptr_t, int, void*);
CreateMove_t oCreateMove = nullptr;

DWORD WINAPI FOVThread(LPVOID lpParam) {
    uintptr_t client = 0;
    while (!g_Unloading) {
        if (Config::bFOVChanger) {
            client = (uintptr_t)GetModuleHandleA("client.dll");
            if (!client) { Sleep(100); continue; }

            __try {
                uintptr_t controller = *(uintptr_t*)(client + offset::dwLocalPlayerController);
                if (controller) {
                    *(int*)(controller + offset::m_iDesiredFOV) = Config::iFOV;
                }
            } __except(1) {}
        }
        Sleep(16);
    }
    return 0;
}

void __stdcall hkCreateMove(uintptr_t rcx, int sequence_number, void* a3) {
    oCreateMove(rcx, sequence_number, a3);

    if (Config::bAimbot || Config::bAutoBhop || Config::bAutoStrafe) {
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (client) {
            __try {
                if (Config::bAimbot) AimbotRun(client);
                if (Config::bAutoBhop || Config::bAutoStrafe) BhopRun(client);
            } __except(1) {}
        }
    }
}

bool GetD3D11Present(void** pPresent) {
    WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, DefWindowProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "CS2_ESP_Dummy", NULL };
    RegisterClassExA(&wc);
    HWND hWnd = CreateWindowA("CS2_ESP_Dummy", "Dummy", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, wc.hInstance, NULL);
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_11_0;
    IDXGISwapChain* sc = nullptr; ID3D11Device* dev = nullptr; ID3D11DeviceContext* ctx = nullptr;
    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &fl, 1, D3D11_SDK_VERSION, &sd, &sc, &dev, NULL, &ctx))) {
        DestroyWindow(hWnd); UnregisterClassA("CS2_ESP_Dummy", wc.hInstance); return false;
    }
    void** vtbl = *reinterpret_cast<void***>(sc);
    *pPresent = vtbl[8];
    sc->Release(); dev->Release(); ctx->Release();
    DestroyWindow(hWnd); UnregisterClassA("CS2_ESP_Dummy", wc.hInstance);
    return true;
}

bool GetCreateMove(void** pCreateMove) {
    uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
    if (!client) return false;

    uintptr_t csgoInput = *(uintptr_t*)(client + offset::dwCSGOInput);
    if (!csgoInput) return false;

    void** vtbl = *reinterpret_cast<void***>(csgoInput);
    *pCreateMove = vtbl[24];
    return true;
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
    if (!g_ImGuiInitialized) {
        if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&g_pd3dDevice))) {
            g_pd3dDevice->GetImmediateContext(&g_pd3dDeviceContext);
            DXGI_SWAP_CHAIN_DESC sd; pSwapChain->GetDesc(&sd);
            g_hWnd = sd.OutputWindow;
            ID3D11Texture2D* bb = nullptr; pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&bb);
            if (bb) { g_pd3dDevice->CreateRenderTargetView(bb, NULL, &g_mainRenderTargetView); bb->Release(); }
            ImGui::CreateContext();
            ImGui_ImplWin32_Init(g_hWnd);
            ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
            GUI_LoadFont(); GUI_Init();
            oWndProc = (WNDPROC)SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)hkWndProc);
            g_ImGuiInitialized = true;
        } else return oPresent(pSwapChain, SyncInterval, Flags);
    }
    if (g_mainRenderTargetView) g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
    ImGui_ImplDX11_NewFrame(); ImGui_ImplWin32_NewFrame(); ImGui::NewFrame();
    GUI_Render();
    if (Config::bBoxESP) {
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (client) {
            DXGI_SWAP_CHAIN_DESC sd; pSwapChain->GetDesc(&sd);
            __try { Overlay::RenderESP(client, sd.BufferDesc.Width, sd.BufferDesc.Height); } __except(1) {}
        }
    }
    {
        uintptr_t client = (uintptr_t)GetModuleHandleA("client.dll");
        if (client) {
            DXGI_SWAP_CHAIN_DESC sd; pSwapChain->GetDesc(&sd);
            __try { Overlay::RenderParticles(sd.BufferDesc.Width, sd.BufferDesc.Height); } __except(1) {}
        }
    }
    ImGui::Render(); ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved) {
    void* pPresent = nullptr;
    if (!GetD3D11Present(&pPresent)) return 0;
    if (MH_Initialize() != MH_OK) return 0;
    if (MH_CreateHook(pPresent, &hkPresent, reinterpret_cast<void**>(&oPresent)) != MH_OK) return 0;

    void* pCreateMove = nullptr;
    if (GetCreateMove(&pCreateMove)) {
        MH_CreateHook(pCreateMove, &hkCreateMove, reinterpret_cast<void**>(&oCreateMove));
    }

    MH_EnableHook(MH_ALL_HOOKS);
    g_hFOVThread = CreateThread(nullptr, 0, FOVThread, nullptr, 0, nullptr);
    while (!GetAsyncKeyState(VK_DELETE)) Sleep(100);
    g_Unloading = true; Sleep(200);
    if (g_hFOVThread) { WaitForSingleObject(g_hFOVThread, 1000); CloseHandle(g_hFOVThread); }
    MH_DisableHook(MH_ALL_HOOKS); MH_RemoveHook(MH_ALL_HOOKS); MH_Uninitialize();
    if (oWndProc) SetWindowLongPtr(g_hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    ImGui_ImplDX11_Shutdown(); ImGui_ImplWin32_Shutdown(); ImGui::DestroyContext();
    return 0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved) {
    if (dwReason == DLL_PROCESS_ATTACH) CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
    return TRUE;
}
