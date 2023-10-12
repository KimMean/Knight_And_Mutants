#include "Framework.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND hWnd;
WCHAR szTitle[] = L"DirectX11_3D_Game";                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[] = L"DirectX11";            // �⺻ â Ŭ���� �̸��Դϴ�.
Vector3 mousePos;
Vector3 screenMousePos;

// ���� ����
ATOM                RegisterWindowClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

/*
* Win32 API ���ø����̼��� ������(entry point) �Լ�
* hInstance: ���ø����̼��� �ν��Ͻ� �ڵ��Դϴ�.
* hPrevInstance: ������ ����Ǿ��� �ν��Ͻ��� �ڵ��Դϴ�. ���� ������ Windows������ �׻� NULL�Դϴ�.
* pCmdLine: ����� �Ű������Դϴ�. �����ڵ� ���ڿ��� ���޵˴ϴ�.
* nCmdShow: �����찡 �ʱ⿡ ǥ�õ� �� ���Ǵ� �÷����Դϴ�.
*/
int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR lpCmdLine,
	int nCmdShow)
{
	// UNREFERENCED_PARAMETER�� �����Ϸ� ��� �����ϱ� ���� ���Ǵ� ��ũ��
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    RegisterWindowClass(hInstance);
    InitInstance(hInstance, nCmdShow);

    // �� ����Ű �ʿ����
    //HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(szWindowClass));

    MSG msg = {};

    GameManager* gameManager = new GameManager();
    // �⺻ �޽��� �����Դϴ�:
    // ���ö����� ���Ž
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
        // �޽��� ť�� Ȯ���ؼ� ������ �����ϰ� ����(PM_REMOVE)������
        // �޽����� �ֵ� ���� ȣ���� ��
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            //if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            TranslateMessage(&msg); // �޽��� �ؼ��ϴ� ����
            DispatchMessage(&msg);  // �޽����� ó���ؾ� �� ���� ������
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
* RegisterWindowClass(HINSTANCE hInstance) �Լ�
* �� �ڵ�� ������ Ŭ������ ����ϴ� �ڵ��Դϴ�.
* WNDCLASSEXW ����ü ���� wcex�� ���� �����ϰ� 
* RegisterClassExW() �Լ��� ���� ������ Ŭ������ ����մϴ�.
*/
ATOM RegisterWindowClass(HINSTANCE hInstance)
{
    // wcex ����ü ������ ���� ������ �� ���� �ʴ� ���̶� 0���� �ʱ�ȭ�� ���־�� �մϴ�.
    // �ʱ�ȭ�� ������ ������ ������ ������ ������ �Ǿ� ������ �߻��մϴ�.
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    // lpfnWndProc�� �ݹ� �Լ� �����͸� �����մϴ�.
    // ������ �޽��� ó���� ���� �ݹ��Լ��� ����Ͽ� ����մϴ�.
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

    // RegisterClass�� �����ϰ� ���� ��
    // RegisterClassExW�� ���� �ɼ� ���� ����
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

    RECT rect = { 0, 0, WIN_WIDTH, WIN_HEIGHT };

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    hWnd = CreateWindowW(
        szWindowClass,          // lpClassName (lpszClassName�̸��� ��ġ�� �ʿ䰡 ����
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

    ShowWindow(hWnd, nCmdShow); // ������ ǥ��
    UpdateWindow(hWnd);         // ������ �׸���

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
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEMOVE:
    {
        // ���콺 ��ġ ���
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