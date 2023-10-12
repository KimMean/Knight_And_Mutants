#include "Framework.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[] = L"DirectX11_3D_Game";                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[] = L"DirectX11";            // 기본 창 클래스 이름입니다.
Vector3 mousePos;
Vector3 screenMousePos;

// 전방 선언
ATOM                RegisterWindowClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

/*
* Win32 API 애플리케이션의 진입점(entry point) 함수
* hInstance: 애플리케이션의 인스턴스 핸들입니다.
* hPrevInstance: 이전에 실행되었던 인스턴스의 핸들입니다. 현재 버전의 Windows에서는 항상 NULL입니다.
* pCmdLine: 명령줄 매개변수입니다. 유니코드 문자열로 전달됩니다.
* nCmdShow: 윈도우가 초기에 표시될 때 사용되는 플래그입니다.
*/
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR lpCmdLine,
	int nCmdShow)
{
	// UNREFERENCED_PARAMETER는 컴파일러 경고를 방지하기 위해 사용되는 매크로
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    RegisterWindowClass(hInstance);
    InitInstance(hInstance, nCmdShow);

    // 난 단축키 필요없음
    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(szWindowClass));

    MSG msg = {};

    GameManager* gameManager = new GameManager();
    // 기본 메시지 루프입니다:
    // 들어올때까지 대기탐
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    //    {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //    }
    //}
    while (msg.message != WM_QUIT)
    {
        // 메시지 큐를 확인해서 있으면 실행하고 지워(PM_REMOVE)버린다
        // 메시지가 있든 없든 호출이 됌
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            TranslateMessage(&msg); // 메시지 해석하는 구문
            DispatchMessage(&msg);  // 메시지를 처리해야 할 곳에 전달함
        }
        else
        {
            gameManager->Update();
            gameManager->Render();
        }
    }

    delete gameManager;

    return (int)msg.wParam;
}


/*
* RegisterWindowClass(HINSTANCE hInstance) 함수
* 이 코드는 윈도우 클래스를 등록하는 코드입니다.
* WNDCLASSEXW 구조체 변수 wcex에 값을 세팅하고 
* RegisterClassExW() 함수를 통해 윈도우 클래스를 등록합니다.
*/
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    // wcex 구조체 변수에 값을 셋팅할 때 쓰지 않는 값이라도 0으로 초기화를 해주어야 합니다.
    // 초기화를 해주지 않으면 쓰레기 값으로 셋팅이 되어 문제가 발생합니다.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // lpfnWndProc는 콜백 함수 포인터를 저장합니다.
    // 윈도우 메시지 처리를 위해 콜백함수를 등록하여 사용합니다.
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX3D));
    wcex.hIcon = LoadIcon(hInstance, IDI_ASTERISK);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(szWindowClass);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(szTitle));

    // RegisterClass는 간단하게 만들 때
    // RegisterClassExW는 상세한 옵션 조절 가능
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    RECT rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    hWnd = CreateWindowW(
        szWindowClass,          // lpClassName (lpszClassName이름과 일치할 필요가 있음
        szTitle,                // lpWindowName
        WS_OVERLAPPEDWINDOW,    // dwStyle
        WIN_START_X,            // x
        WIN_START_Y,            // y
        WIN_WIDTH,              // nWidth
        WIN_HEIGHT,             // nHeight
        nullptr,                // nWndParent
        nullptr,                // nMenu
        hInstance,              // hInstance
        nullptr                 // lpParam
    );               

    if (!hWnd) return FALSE;

    ShowWindow(hWnd, nCmdShow); // 윈도우 표시
    UpdateWindow(hWnd);         // 윈도우 그리기

    return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEMOVE:
    {
        // 마우스 위치 사용
        //RECT clientRect;
        //GetClientRect(hWnd, &clientRect);
        mousePos.x = LOWORD(lParam);
        mousePos.y = WIN_HEIGHT - (float)HIWORD(lParam);

        screenMousePos = mousePos;
        screenMousePos.x += mousePos.x / WIN_WIDTH * 20;
        screenMousePos.y += (WIN_HEIGHT - mousePos.y) / WIN_HEIGHT * -40;
        //mousePos.y = clientRect.bottom - HIWORD(lParam);

        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}