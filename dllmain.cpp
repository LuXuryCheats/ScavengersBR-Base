
#include "Menu.h"




ID3D11Device* device = nullptr;
ID3D11DeviceContext* immediateContext = nullptr;
ID3D11RenderTargetView* renderTargetView = nullptr;

HRESULT(*presenth)(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) = nullptr;
HRESULT(*resizeh)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) = nullptr;

WNDPROC oriWndProc = NULL;
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int Screen_X, Screen_Y;
HWND hwnd;

void OutlinedText(ImGuiWindow& windowshit, std::string str, ImVec2 loc, ImU32 colr, bool centered = false)
{
	ImVec2 size = { 0,0 };
	float minx = 0;
	float miny = 0;
	if (centered)
	{
		size = ImGui::GetFont()->CalcTextSizeA(windowshit.DrawList->_Data->FontSize, 0x7FFFF, 0, str.c_str());
		minx = size.x / 2.f;
		miny = size.y / 2.f;
	}

	windowshit.DrawList->AddText(ImVec2((loc.x - 1) - minx, (loc.y - 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
	windowshit.DrawList->AddText(ImVec2((loc.x + 1) - minx, (loc.y + 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
	windowshit.DrawList->AddText(ImVec2((loc.x + 1) - minx, (loc.y - 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
	windowshit.DrawList->AddText(ImVec2((loc.x - 1) - minx, (loc.y + 1) - miny), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), str.c_str());
	windowshit.DrawList->AddText(ImVec2(loc.x - minx, loc.y - miny), colr, str.c_str());
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) && Config::Menu == true)
	{
		return true;
	}
	return CallWindowProc(oriWndProc, hWnd, msg, wParam, lParam);
}

namespace R
{
	float color_red = 1.0;
	float color_green = 0;
	float color_blue = 0;
	float color_random = 0.0;
	float color_speed = -2.0;
	void ColorChange()
	{
		static float Color[3];
		static DWORD Tickcount = 0;
		static DWORD Tickcheck = 0;
		ImGui::ColorConvertRGBtoHSV(color_red, color_green, color_blue, Color[0], Color[1], Color[2]);
		if (GetTickCount64() - Tickcount >= 1)
		{
			if (Tickcheck != Tickcount)
			{
				Color[0] += 0.001f * color_speed;
				Tickcheck = Tickcount;
			}
			Tickcount = GetTickCount64();
		}
		if (Color[0] < 0.0f) Color[0] += 1.0f;
		ImGui::ColorConvertHSVtoRGB(Color[0], Color[1], Color[2], color_red, color_green, color_blue);
	}
}

// Note : Failed
void WigglyText_Test(ImDrawVert* vert_start, ImDrawVert* vert_end, float phase, float speed, float amplitude)
{
	for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
		vertex->pos.y += sinf(vertex->pos.x * speed + phase) * amplitude;
}

// Font Names

ImFont* SmallText;
ImFont* MediumText;
ImFont* LargeText;
ImFont* Arial;
ImFont* ArialLarge;
ImFont* Symbols;
ImFont* Regular;
ImFont* RegularLarge;

// Pos X + Pos Y

int X, Y;

HRESULT present_hk(IDXGISwapChain* swapchain, UINT sync, UINT flags)
{
	if (!device)
	{
		ID3D11Texture2D* renderTarget = 0;
		ID3D11Texture2D* backBuffer = 0;
		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		swapchain->GetDevice(__uuidof(device), (PVOID*)&device);
		device->GetImmediateContext(&immediateContext);

		swapchain->GetBuffer(0, __uuidof(renderTarget), (PVOID*)&renderTarget);
		device->CreateRenderTargetView(renderTarget, nullptr, &renderTargetView);
		renderTarget->Release();
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (PVOID*)&backBuffer);
		backBuffer->GetDesc(&backBufferDesc);

		Screen_X = backBufferDesc.Width;
		Screen_Y = backBufferDesc.Height;

		backBuffer->Release();
		if (!hwnd)
		{
			hwnd = FindWindowW(L"UnrealWindow", L"Scavenger  ");

			if (!hwnd)
				hwnd = GetForegroundWindow();
		}
		// Main Fonts
		ArialLarge = ImGui::GetIO().Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\arial.ttf"), 20.0f);
		Arial = ImGui::GetIO().Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\arial.ttf"), 12.0f);
		Regular = ImGui::GetIO().Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\tahoma.ttf"), 16.0f);
		RegularLarge = ImGui::GetIO().Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\tahoma.ttf"), 30.0f);

		X = (float)backBufferDesc.Width;
		Y = (float)backBufferDesc.Height;
		ImGui_ImplDX11_Init(hwnd, device, immediateContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	auto& window = BeginScene();

	R::ColorChange();

	if (Settings::FOV)
	{
		window.DrawList->AddCircle(ImVec2(X / 2, Y / 2), Settings::Size - 1, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 70, 1.f);
		window.DrawList->AddCircle(ImVec2(X / 2, Y / 2), Settings::Size + 1, ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 70, 1.f);
		window.DrawList->AddCircle(ImVec2(X / 2, Y / 2), Settings::Size, ImColor(R::color_red, R::color_green, R::color_blue), 70, 1.5f);
	}

	ImGui::PushFont(RegularLarge);
	OutlinedText(window, ("Scavenger Private"), ImVec2(70, 70), ImColor(R::color_red, R::color_green, R::color_blue));
	ImGui::PopFont();

	ImGui::PushFont(Arial);
	//OutlinedText(window, ("MENU KEY : INSERT"), ImVec2(70, 120), ImColor(255, 0, 81));
	ImGui::PopFont();

	ImGui::PushFont(Regular);
	Config::MenuINIT();


	EndScene(window);
	return presenth(swapchain, sync, flags);
	ImGui::PopFont();
}


HRESULT resize_hk(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
	ImGui_ImplDX11_Shutdown();
	renderTargetView->Release();
	immediateContext->Release();
	device->Release();
	device = nullptr;
	return resizeh(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

ImGuiWindow& BeginScene() {
	ImGui_ImplDX11_NewFrame();
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));
	ImGui::Begin("##scene", nullptr, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoTitleBar);
	auto& io = ImGui::GetIO();
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
	return *ImGui::GetCurrentWindow();
}

VOID EndScene(ImGuiWindow& window) {
	window.DrawList->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::Render();
}


VOID hook() {

	Sleep(1000);

	HWND window = FindWindow(0, (L"Scavenger  "));

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	auto                 featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.OutputWindow = window;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
		MessageBox(0, (L"dx11 failed hook"), (L"fatal error"), MB_ICONERROR);
		return;
	}

	auto table = *reinterpret_cast<PVOID**>(swapChain);
	auto present = table[8];
	auto resize = table[13];

	context->Release();
	device->Release();
	swapChain->Release();

	MH_Initialize();

	MH_CreateHook(present, present_hk, reinterpret_cast<PVOID*>(&presenth));
	MH_EnableHook(present);

	MH_CreateHook(resize, resize_hk, reinterpret_cast<PVOID*>(&resizeh));
	MH_EnableHook(resize);

	Beep(400, 400);

	oriWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		hook();
	}

	return TRUE;
}