#include <windows.h>

import lysa;
import vireo;

lysa::SurfaceConfig surfaceConfig {
    .backend = vireo::Backend::VULKAN,
    .msaa = vireo::MSAA::X4,
};

lysa::ApplicationConfig applicationConfig {
    .appDir = "..",
    .defaultFontName = "app://res/fonts/MontserratMedium.otf",
    .defaultFontSize = 30,
    .loggingMode = lysa::LOGGING_MODE_FILE | lysa::LOGGING_MODE_STDOUT,
};

constexpr auto WIDTH{1280};
constexpr auto HEIGHT{720};

bool dirExists(const std::string& dirName_in) {
    const DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES) && (ftyp & FILE_ATTRIBUTE_DIRECTORY);
}

LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    if (!dirExists("shaders")) {
        MessageBox(nullptr,
                   L"Shaders directory not found, please run the application from the root of the project",
                   L"Error",
                   MB_OK);
        return 0;
    }

    std::wstring title{L"LysaGame"};
    if (surfaceConfig.backend == vireo::Backend::VULKAN) {
        title.append(L" (Vulkan)");
    } else {
        title.append(L" (DirectX)");
    }
    const auto windowClass = WNDCLASSEX{
        .cbSize = sizeof(WNDCLASSEX),
        .style = CS_HREDRAW | CS_VREDRAW,
        .lpfnWndProc = WindowProc,
        .hInstance = hInstance,
        .hCursor = LoadCursor(nullptr, IDC_ARROW),
        .lpszClassName = L"LysaGameWindowClass",
    };
    RegisterClassEx(&windowClass);

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    int w = WIDTH;
    int h = HEIGHT;
    DWORD style;
    DWORD exStyle;
    if (w == 0 || h == 0) {
        exStyle = WS_EX_APPWINDOW;
        style = WS_POPUP | WS_MAXIMIZE;
        // EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorData));
        // w = monitorData.monitorRect.right - monitorData.monitorRect.left;
        // h = monitorData.monitorRect.bottom - monitorData.monitorRect.top;
        // x = monitorData.monitorRect.left;
        // y = monitorData.monitorRect.top;
    } else {
        style = WS_OVERLAPPEDWINDOW;
        exStyle = 0;
        auto windowRect = RECT{0, 0, static_cast<LONG>(w), static_cast<LONG>(h)};
        AdjustWindowRect(&windowRect, style, FALSE);
        x = (GetSystemMetrics(SM_CXSCREEN) - (windowRect.right - windowRect.left)) / 2;
        y = (GetSystemMetrics(SM_CYSCREEN) - (windowRect.bottom - windowRect.top)) / 2;
    }

    const auto hwnd = CreateWindowEx(
        exStyle,
        windowClass.lpszClassName,
        title.c_str(),
        style,
        x, y,
        w, h,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    try {
        auto app = lysa::Application{applicationConfig};

        ShowWindow(hwnd, nCmdShow);
        auto msg = MSG{};
        while (msg.message != WM_QUIT) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        return static_cast<char>(msg.wParam);
    } catch (vireo::Exception& e) {
        MessageBoxA(nullptr, e.what(), "Fatal error", MB_OK);
        return 1;
    }
}