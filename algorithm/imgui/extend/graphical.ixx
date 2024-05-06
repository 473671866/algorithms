module;
#include <iostream>
#include <windows.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#pragma comment(lib, "d3d11.lib")

#pragma warning(push)
#pragma warning(disable: 5202)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#pragma warning(pop)

export module graphical;
import component;
namespace imgui = ImGui;

export namespace view {
	class graphiacl {
	private:
		ID3D11Device* _device = nullptr;
		ID3D11DeviceContext* _context = nullptr;
		IDXGISwapChain* _swapchain = nullptr;
		ID3D11RenderTargetView* _render = nullptr;
		ImGuiWindowFlags _flags = ImGuiWindowFlags_None;
		std::string _name = "example";
		component* _com;
		HWND _hwnd;
		ImVec2 _wndsize;

	public:

		~graphiacl() {
			if (_render) {
				_render->Release();
				_render = nullptr;
			}

			if (_swapchain) {
				_swapchain->Release();
				_swapchain = nullptr;
			}

			if (_context) {
				_context->Release();
				_context = nullptr;
			}
			if (_device) {
				_device->Release();
				_device = nullptr;
			}

			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			imgui::DestroyContext();
		}

		bool external(std::wstring_view title, ImVec2 wndsize = { 720, 480 })  noexcept(false) {
			_wndsize = wndsize;
			WNDCLASSEXW wc = { sizeof(wc),
				CS_CLASSDC,
				handler,
				0L, 0L,
				GetModuleHandle(nullptr),
				nullptr, nullptr, nullptr, nullptr,
				L"ImGui Example",
				nullptr };
			RegisterClassExW(&wc);

			_hwnd = CreateWindowW(wc.lpszClassName, title.data(), WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX, 100, 100, (int)_wndsize.x, (int)_wndsize.y, nullptr, nullptr, wc.hInstance, nullptr);
			ShowWindow(_hwnd, SW_SHOWDEFAULT);
			UpdateWindow(_hwnd);

			DXGI_SWAP_CHAIN_DESC desc = { };
			desc.BufferCount = 2;
			desc.BufferDesc.Width = 0;
			desc.BufferDesc.Height = 0;
			desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.BufferDesc.RefreshRate.Numerator = 60;
			desc.BufferDesc.RefreshRate.Denominator = 1;
			desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.OutputWindow = _hwnd;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Windowed = TRUE;
			desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

			std::uint32_t flags = 0;
			D3D_FEATURE_LEVEL level = { };
			D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

			HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				flags,
				levels,
				2,
				D3D11_SDK_VERSION,
				&desc,
				&_swapchain,
				&_device,
				&level,
				&_context);

			if (res != S_OK) {
				throw std::exception("failed to create device and swapchain");
				return false;
			}

			ID3D11Texture2D* buffer = nullptr;
			_swapchain->GetBuffer(0, IID_PPV_ARGS(&buffer));
			_device->CreateRenderTargetView(buffer, nullptr, &_render);
			buffer->Release();

			imgui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui_ImplWin32_Init(_hwnd);
			ImGui_ImplDX11_Init(_device, _context);
			return true;
		}

		void name(std::string_view name) {
			_name = name;
		}

		void flags(ImGuiWindowFlags flag) {
			_flags = flag;
		}

		ImGuiStyle& styles() {
			return imgui::GetStyle();
		}

		bool principal(component* com) {
			_com = com;
			return true;
		}

		bool startup() {
			ImVec4 color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			float alphas[4] = { color.x * color.w, color.y * color.w, color.z * color.w, color.w };
			ImVec2 pos = { 0,0 };
			while (true) {
				MSG msg;
				if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (msg.message == WM_QUIT) {
					break;
				}

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				imgui::NewFrame();

				imgui::Begin(_name.data(), nullptr, _flags);
				imgui::SetWindowSize(_wndsize);
				imgui::SetWindowPos(pos);
				_com->domain();
				imgui::End();
				imgui::Render();
				_context->OMSetRenderTargets(1, &_render, nullptr);
				_context->ClearRenderTargetView(_render, alphas);
				ImGui_ImplDX11_RenderDrawData(imgui::GetDrawData());

				_swapchain->Present(1, 0);
			}
			return true;
		}

		static LRESULT WINAPI handler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}

			if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
				return true;
			}

			return ::DefWindowProcW(hWnd, msg, wParam, lParam);
		}
	};
}