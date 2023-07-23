// Game2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Game2.h"

#define MAX_LOADSTRING 100

using namespace std;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.


//함수
void Update();
void DrawDoubleBuffering(HDC& hdc);


#ifdef UNICODE

#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 

#else

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 

#endif



// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAME2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAME2));

    MSG msg;

    // 기본 메시지 루프입니다:

    static clock_t oldtime = clock();
    static clock_t newtime;

    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            newtime = clock();
            if (newtime - oldtime >= 34)
            {
                Update();
                oldtime = newtime;
            }
        }
    }

    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAME2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
static HWND hWnd;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
       600, 200, 804, 799, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

//전역 변수
#define MAPSIZE 10 //맵 크기 설정

HDC mem1dc; // 더블 버퍼링

RECT rectView; //화면 크기

static HBITMAP hBit, oldBit; // 더블 버퍼링 비트맵
static Drawer player;

static vector<POINT> movepoints;

static int linedirection = -1;//선을 그리는 방향
static int startpointdirection = -1;//그리는 시작점에서의 방향

static vector<POINT> ReturnLines;//스페이스바를 뗄 때 선 위에 있지 않으면 왔던 길을 되돌아와야됨


static list<POINT> Area;//플레이어가 차지한 공간

static int cannotdraw = 0;

static POINT vertex[4];


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        player.setX(rectView.left + MAPSIZE);
        player.setY(rectView.top + MAPSIZE);
        player.setWidth(20);
        player.setHeight(20);
        player.setSpeed(8);
        Area.push_back({ rectView.left + MAPSIZE, rectView.bottom - MAPSIZE });
        Area.push_back({ rectView.right - MAPSIZE, rectView.bottom - MAPSIZE });
        Area.push_back({ rectView.right - MAPSIZE, rectView.top + MAPSIZE });
        Area.push_back({ rectView.left + MAPSIZE, rectView.top + MAPSIZE });
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            DrawDoubleBuffering(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//메소드
void Update()
{
    vector<int> OnAreaLines;//그려진 도형의 선 위에 있는 지
    vector<int> BeforeOnAreaLines;//직전 좌표가 그려진 도형의 선 위에 있는 지

    int onarealinesize = 0;//플레이어가 도형 위에 있는 선의 갯수
    int beforeonarealinesize = 0;

    int divecheck = 0;

    InvalidateRect(hWnd, NULL, FALSE);
    //연산 직전의 플레이어의 좌표
    int BeforeX = player.getX();
    int BeforeY = player.getY();
    OnAreaLineCheck(Area, player, BeforeOnAreaLines);

    

    if (cannotdraw == 0)
    {
        //이동
        if (GetKeyState(VK_LEFT) < 0)
        {
            player.setX(player.getX() - player.getSpeed());
            player.setDirection(3);
        }
        else if (GetKeyState(VK_RIGHT) < 0)
        {
            player.setX(player.getX() + player.getSpeed());
            player.setDirection(1);
        }
        else if (GetKeyState(VK_DOWN) < 0)
        {
            player.setY(player.getY() + player.getSpeed());
            player.setDirection(0);
        }
        else if (GetKeyState(VK_UP) < 0)
        {
            player.setY(player.getY() - player.getSpeed());
            player.setDirection(2);
        }
        else
            player.setDirection(-1);//방향이 존재하지 않는다
    }


    //스페이스바의 눌림에 따른 기능
    if (GetKeyState(VK_SPACE) < 0)//땅따먹기 도형 그리기
    {
        if (startpointdirection == -1 && movepoints.size() == 1)//출발점의 방향 설정
        {
            if (player.getX() - movepoints[0].x > 0)
                startpointdirection = 1;
            else if (player.getX() - movepoints[0].x < 0)
                startpointdirection = 3;
            else if (player.getY() - movepoints[0].y < 0)
                startpointdirection = 2;
            else if (player.getY() - movepoints[0].y > 0)
                startpointdirection = 0;
        }

        if (player.getReturning() == 1)//돌아가고 있을 때는 새로운 행동을 할 수 없다.
        {
            player.setX(BeforeX);
            player.setY(BeforeY);
            return;
        }
        OnlyOnWindow(MAPSIZE, player, rectView);
        if (player.getDrawing() == 0)//맨 처음 시작점
        {
            movepoints.push_back({ BeforeX, BeforeY });
            linedirection = player.getDirection();
        }
        player.setDrawing(1);
        if (!(player.getDirection() == -1 || player.getDirection() == linedirection//전 위치로 이동
            || player.getDirection() == linedirection - 2 || player.getDirection() == linedirection + 2))
        {
            movepoints.push_back({ BeforeX, BeforeY });
            linedirection = player.getDirection();//시작 경우
        }

        if (SelfLineCheck(movepoints, player, BeforeX, BeforeY) == true)//자신의 선에 충돌하는지 확인
        {
            player.setX(movepoints[0].x);
            player.setY(movepoints[0].y);
            movepoints.clear();
            cannotdraw = 1;
            return;
        }


        OnAreaLineCheck(Area, player, OnAreaLines);




        if (OnAreaLines.size() >= 1)//땅 점령후 도형 생성
        {

            movepoints.push_back({ player.getX(), player.getY() });
            

            player.setDrawing(0);

            if (movepoints.size() < 2)
            {
                player.setX(movepoints[0].x);
                player.setY(movepoints[0].y);
            }
            if (movepoints.size() >= 2 && (movepoints[0].x != movepoints[movepoints.size() - 1].x || movepoints[0].y != movepoints[movepoints.size() - 1].y)
                && BeforeOnAreaLines.size() == 0)
            {
                int readingdirection = 0;
                player.setX(movepoints[movepoints.size() - 1].x);
                player.setY(movepoints[movepoints.size() - 1].y);
                list<POINT> templist;
                list<POINT> redesignlist;
                list<POINT> lastlist;
                for (int i = 0; i < movepoints.size(); i++)//지나왔던 점들을 전부 저장
                    templist.push_back(movepoints[i]);
                list<POINT>::iterator iter = templist.begin();
                startpointdirection = -1;

                RedesignList(Area, templist, redesignlist, movepoints[0], movepoints[movepoints.size() - 1]);//리스트 재배열

                SumAreas(Area, redesignlist);

                cannotdraw = 1;
            }
                movepoints.clear();
        }
    }
    else//도형과 맵 테두리 이동
    {
        cannotdraw = 0;
        if (player.getDrawing() == 1)//스페이스바를 떼면 되돌아가야함
        {
            if (movepoints.size() > 0)
            {
                player.setDrawing(0);
                linedirection = -1;

                for (int i = 0; i < movepoints.size(); i++)
                    ReturnLines.push_back(movepoints[i]);//돌아가기위한 선들

                player.setReturning(1);
                movepoints.clear();
            }
        }

        if (player.getReturning() == 0)//돌아가고 있는 상태가 아닐 때
        {
            OnAreaLineCheck(Area, player, OnAreaLines);
            int linecheck = 0;
           
            if (OnAreaLines.size() < 1)
            {
                player.setX(BeforeX);
                player.setY(BeforeY);
            }
        }
        else//돌아가는 상태일 때
        {
            int lastpointx = ReturnLines[ReturnLines.size() - 1].x;
            int lastpointy = ReturnLines[ReturnLines.size() - 1].y;

            if (lastpointx == player.getX() && lastpointy == player.getY())
                ReturnLines.pop_back();
            else
                PlayerCorrectOnLine(lastpointx, lastpointy, BeforeX, BeforeY, player);

            if (ReturnLines.size() <= 0)
                player.setReturning(0);
        }
    }

}

void DrawDoubleBuffering(HDC& hdc)
{
    mem1dc = CreateCompatibleDC(hdc);
    if (hBit == NULL)
        hBit = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    oldBit = (HBITMAP)SelectObject(mem1dc, hBit);
    FillRect(mem1dc, &rectView, GetSysColorBrush(COLOR_WINDOW));

    POINT playerpoint = { player.getX(), player.getY() };


    POINT* temp = new POINT[Area.size() + 1];
    HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(192, 192, 192));
    HBRUSH oldBrush = (HBRUSH)SelectObject(mem1dc, myBrush);
    int i = 0;
    for (POINT x : Area)//도형을 그림
    {
        temp[i++] = x;
    }
    Polygon(mem1dc, temp, Area.size());
    SelectObject(mem1dc, oldBrush);
    DeleteObject(myBrush);
    delete[] temp;


    for (int i = 0; i < movepoints.size(); i++)//이동한 선을 그림
    {
        if(i == movepoints.size() - 1)
            DrawLine(mem1dc, movepoints[i], playerpoint);
        else
            DrawLine(mem1dc, movepoints[i], movepoints[i + 1]);
    }

    for (int i = 0; i < ReturnLines.size(); i++)
    {
        if (i == ReturnLines.size() - 1)
            DrawLine(mem1dc, ReturnLines[i], playerpoint);
        else
            DrawLine(mem1dc, ReturnLines[i], ReturnLines[i + 1]);
    }

    DrawRectangle(mem1dc, player);

    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, mem1dc, 0, 0, SRCCOPY);
    SelectObject(mem1dc, oldBit);
    DeleteDC(mem1dc);
}





// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
