#include <windows.h>

bool dirExists(const std::string& dirName_in) {
    const DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    return (ftyp != INVALID_FILE_ATTRIBUTES) && (ftyp & FILE_ATTRIBUTE_DIRECTORY);
}

LRESULT CALLBACK WindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam) {
    auto* surface = reinterpret_cast<lysa::Surface*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    switch (message) {
    case WM_SIZE:
        surface->resize();
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    default:;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

struct MonitorEnumData {
    int  enumIndex{0};
    int  monitorIndex{0};
    RECT monitorRect{};
};

BOOL CALLBACK MonitorEnumProc(HMONITOR, HDC , const LPRECT lprcMonitor, const LPARAM dwData) {
    const auto data = reinterpret_cast<MonitorEnumData*>(dwData);
    if (data->enumIndex == data->monitorIndex) {
        data->monitorRect = *lprcMonitor;
        return FALSE;
    }
    data->enumIndex++;
    return TRUE;
}

int WINAPI WinMain(const HINSTANCE hInstance, const HINSTANCE, const LPSTR, const int nShowCmd) {
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
        auto monitorRect = RECT{};
        const auto hPrimary = MonitorFromWindow(nullptr, MONITOR_DEFAULTTOPRIMARY);
        auto monitorInfo = MONITORINFOEX{};
        monitorInfo.cbSize = sizeof(MONITORINFOEX);
        if (GetMonitorInfo(hPrimary, &monitorInfo)) {
            monitorRect = monitorInfo.rcMonitor;
        } else {
            auto monitorData = MonitorEnumData {};
            EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitorData));
            monitorRect = monitorData.monitorRect;
        }
        w = monitorRect.right - monitorRect.left;
        h = monitorRect.bottom - monitorRect.top;
        x = monitorRect.left;
        y = monitorRect.top;
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
        const auto app = lysa::Application{applicationConfig};
        const auto surface = app.createSurface(surfaceConfig, hwnd);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(surface.get()));
        ShowWindow(hwnd, nShowCmd);
        auto msg = MSG{};
        while (msg.message != WM_QUIT) {
            surface->drawFrame();
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
