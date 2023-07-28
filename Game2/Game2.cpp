// Game2.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#define _USE_MATH_DEFINES
#include <ole2.h>
#include "framework.h"
#include "Game2.h"
#include <gdiplus.h>

using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib") 


ULONG_PTR m_gdiplusToken;

#define MAX_LOADSTRING 100

using namespace std;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

//함수
void Update();
void DrawDoubleBuffering(HDC& hdc);


static Image* image;//이미지

static Image* background;//배경화면
static Image* life;//체력
static Image* cursor;
static Image* wall;
static TextureBrush* myBrush;
static TextureBrush* wallBrush;
static Image* explode;

static int fontcolor = 0;

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

    clock_t oldtime = clock();
    clock_t newtime;
    clock_t oldanitime = clock();
    clock_t newanitime;
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
            newanitime = clock();
            if (newtime - oldtime >= 34)
            {
                oldtime = newtime;
                Update();
            }

            if (newanitime - oldanitime >= 100)
            {
                oldanitime = newanitime;
                (*image).RotateFlip(Rotate90FlipNone);
                fontcolor = 1 - fontcolor;
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
   GdiplusStartupInput gdiplusStartupInput;
   GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);
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
static int page = 0;
static HBITMAP hBit, oldBit; // 더블 버퍼링 비트맵
static Drawer player;

static vector<POINT> movepoints;

static int linedirection = -1;//선을 그리는 방향
static int startpointdirection = -1;//그리는 시작점에서의 방향

static vector<POINT> ReturnLines;//스페이스바를 뗄 때 선 위에 있지 않으면 왔던 길을 되돌아와야됨


static list<POINT> Area;//플레이어가 차지한 공간

static int cannotdraw = 0;

static POINT vertex[4];

static int FullArea;

static double PerArea;

static vector<ObjectCircle> circles;//오브젝트

static clock_t oldanimationtimer = clock();
static clock_t newanimationtimer;

static Point* temp;

static int backgroundupdate = 1;

static int explodeframeX = 0;
static int explodeframeY = 0;
static vector<POINT> explodepoints;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        GetClientRect(hWnd, &rectView);
        player.setX(rectView.left + MAPSIZE);
        player.setY(rectView.top + MAPSIZE);
        player.setWidth(20);
        player.setHeight(20);
        player.setSpeed(8);
        player.setLife(3);

        Area.push_back({ rectView.left + MAPSIZE, rectView.bottom - MAPSIZE });
        Area.push_back({ rectView.right - MAPSIZE, rectView.bottom - MAPSIZE });
        Area.push_back({ rectView.right - MAPSIZE, rectView.top + MAPSIZE });
        Area.push_back({ rectView.left + MAPSIZE, rectView.top + MAPSIZE });
        for (int i = 0; i < 6; i++)
        {
            ObjectCircle circle;
            circle.setX((rectView.left + rectView.right) / 2);
            circle.setY((rectView.bottom + rectView.top) / 2);
            circle.setRadius(20);
            circle.setSpeed(6);
            circle.setAngle(randomize(0, 359));
            circles.push_back(circle);
        }
        temp = new Point[Area.size() + 1];
        int i = 0;

        for (POINT x : Area)//도형을 그림
        {
            temp[i].X = x.x;
            temp[i].Y = x.y;
            i++;
        }

        FullArea = GetArea(Area);
        PerArea = 1;

        image = Image::FromFile(L"images/1.png");
        background = Image::FromFile(L"images/2.jpg");
        life = Image::FromFile(L"images/3.png");
        cursor = Image::FromFile(L"images/4.png");
        wall = Image::FromFile(L"images/5.png");

        myBrush = new TextureBrush(background);
        wallBrush = new TextureBrush(wall);

        explode = Image::FromFile(L"images/explode.png");

    }
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
    case WM_CHAR:
        if (wParam == 's')
        {
            if(page != 1)
                page++;
        }
        if (wParam == 'p')
            page++;
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
    if (page == 1)
    {
        //오브젝트 이동
        for (int i = 0; i < circles.size(); i++)
        {
            CollideCheck(Area, circles[i],circles[i].getSpeed());
            if (movepoints.size() > 0 || player.getReturning() == 1)
            {
                if (player.getReturning() == 0)
                {
                    if (HitPlayer(movepoints, circles[i], player) == true)
                    {
                        player.setLife(player.getLife() - 1);
                        player.setX(movepoints[0].x);
                        player.setY(movepoints[0].y);
                        movepoints.clear();
                        cannotdraw = 1;
                        if (player.getLife() <= 0)
                        {
                            page++;
                            return;
                        }
                        return;
                    }
                }
                else if (player.getReturning() == 1)
                {
                    if (HitPlayer(ReturnLines, circles[i], player) == true)
                    {
                        player.setLife(player.getLife() - 1);
                        player.setX(ReturnLines[0].x);
                        player.setY(ReturnLines[0].y);
                        ReturnLines.clear();
                        player.setReturning(0);
                        cannotdraw = 1;
                        if (player.getLife() <= 0)
                        {
                            page++;
                            return;
                        }
                        return;
                    }
                }
            }
        }
        vector<int> OnAreaLines;//그려진 도형의 선 위에 있는 지
        vector<int> BeforeOnAreaLines;//직전 좌표가 그려진 도형의 선 위에 있는 지

        int onarealinesize = 0;//플레이어가 도형 위에 있는 선의 갯수
        int beforeonarealinesize = 0;

        int divecheck = 0;

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

            OnAreaLineCheck(Area, player, OnAreaLines);

            if (player.getReturning() == 1)
            {
                player.setX(BeforeX);
                player.setY(BeforeY);
                return;
            }

            if (OnArea(Area, player) == false && OnAreaLines.size() == 0)
            {
                player.setX(BeforeX);
                player.setY(BeforeY);
            }
            OnlyOnWindow(MAPSIZE, player, rectView);
            if (player.getDrawing() == 0)//맨 처음 시작점
            {
                movepoints.push_back({ BeforeX, BeforeY });
                linedirection = player.getDirection();
                backgroundupdate = 1;
            }
            player.setDrawing(1);
            if (!(player.getDirection() == -1 || player.getDirection() == linedirection//전 위치로 이동
                || player.getDirection() == linedirection - 2 || player.getDirection() == linedirection + 2))
            {
                movepoints.push_back({ BeforeX, BeforeY });

                if (movepoints[movepoints.size() - 1].x == movepoints[movepoints.size() - 2].x
                    && movepoints[movepoints.size() - 1].y == movepoints[movepoints.size() - 2].y)
                {
                    movepoints.erase(movepoints.begin() + movepoints.size() - 1);
                    movepoints.erase(movepoints.begin() + movepoints.size() - 1);
                }

                linedirection = player.getDirection();//시작 경우
            }

            if (SelfLineCheck(movepoints, player, BeforeX, BeforeY) == true)//자신의 선에 충돌하는지 확인
            {
                player.setX(movepoints[0].x);
                player.setY(movepoints[0].y);
                player.setLife(player.getLife() - 1);
                if (player.getLife() == 0)
                {
                    page++;
                    return;
                }
                movepoints.clear();
                cannotdraw = 1;
                return;
            }

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

                    for (int i = 0; i < circles.size(); i++)
                    {
                        if (OnArea(Area, circles[i]) == false)
                        {
                            POINT temp = { circles[i].getX(), circles[i].getY()};
                            explodepoints.push_back(temp);
                            circles.erase(circles.begin() + i);
                            i = -1;
                        }
                    }

                    delete[] temp;

                    temp = new Point[Area.size() + 1];
                    int i = 0;
                    for (POINT x : Area)//도형을 그림
                    {
                        temp[i].X = x.x;
                        temp[i].Y = x.y;
                        i++;
                    }

                    cannotdraw = 1;

                    PerArea = (double)GetArea(Area) / (double)FullArea;

                    if (PerArea < 0.2)
                        page++;
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
                int check = 0;

                if (OnAreaLines.size() < 1)
                {
                    player.setX(BeforeX);
                    player.setY(BeforeY);
                }

                for (int i = 0; i < OnAreaLines.size(); i++)
                {
                    for (int j = 0; j < BeforeOnAreaLines.size(); j++)
                    {
                        if (OnAreaLines[i] == BeforeOnAreaLines[j])
                        {
                            check = 1;
                            break;
                        }
                    }
                }

                if (check == 0)
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
 
    InvalidateRect(hWnd, NULL, FALSE);
}
void DrawDoubleBuffering(HDC& hdc)
{
    TCHAR temptchar[30];
    mem1dc = CreateCompatibleDC(hdc);
    if (hBit == NULL)
        hBit = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
    oldBit = (HBITMAP)SelectObject(mem1dc, hBit);
    SetTextAlign(mem1dc, TA_CENTER);
    Graphics g(mem1dc);
    Rect rect;
    if (page == 1)
    {

        POINT playerpoint = { player.getX(), player.getY() };
        rect.X = 0;
        rect.Y = 0;
        rect.Width = rectView.right;
        rect.Height = rectView.bottom;
        g.FillRectangle(myBrush, rect);

        g.FillPolygon(wallBrush, temp, Area.size());
        /*
        HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(192, 192, 192));
        HBRUSH oldBrush = (HBRUSH)SelectObject(mem1dc, myBrush);
        Polygon(mem1dc, temp, Area.size());
        SelectObject(mem1dc, oldBrush);
        DeleteObject(myBrush);
        */

        HPEN hNewPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
        HPEN hOldPen = (HPEN)SelectObject(mem1dc, hNewPen);




        for (int i = 0; i < movepoints.size(); i++)//이동한 선을 그림
        {
            if (i == movepoints.size() - 1)
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

        _stprintf_s(temptchar, L"LeftArea : %.1lf", PerArea * 100);

        Font font(L"Arial", 20, FontStyleBold, UnitPixel);
        SolidBrush sbrush(Color::Black);
        StringFormat format;

        format.SetAlignment(StringAlignmentNear);
        format.SetLineAlignment(StringAlignmentCenter);
        g.DrawString(temptchar, _tcslen(temptchar),&font,RectF(20, 0, 200, 50), &format, &sbrush);
        //TextOut(mem1dc, 100, 100, temptchar, _tcslen(temptchar));

        for (int i = 0; i < player.getLife(); i++)
        {
            rect.X = 20 + i * 25;
            rect.Y = 40;
            rect.Width = 25;
            rect.Height = 20;
            g.DrawImage(life, rect);
        }
        rect.X = player.getX() - player.getWidth() * 0.6;
        rect.Y = player.getY() - player.getWidth() * 0.6;
        rect.Width = player.getWidth() * 1.2;
        rect.Height = player.getHeight() * 1.2;

        g.DrawImage(cursor, rect);

        for (int i = 0; i < circles.size(); i++)
        {
            rect.X = circles[i].getX() - circles[i].getRadius();
            rect.Y = circles[i].getY() - circles[i].getRadius();
            rect.Width = circles[i].getRadius() * 2;
            rect.Height = circles[i].getRadius() * 2;
            g.DrawImage(image, rect);
        }

        if (explodepoints.size() > 0)
        {
            for (int i = 0; i < explodepoints.size(); i++)
            {
                rect.X = explodepoints[i].x - 40;
                rect.Y = explodepoints[i].y - 40;
                rect.Width = 80;
                rect.Height = 80;

                g.DrawImage(explode, rect, explodeframeX * 100, explodeframeY * 100, 100, 100, UnitPixel);

            }
            explodeframeX++;
            if (explodeframeX >= 10)
            {
                explodeframeX = 0;
                explodeframeY++;
            }

            if (explodeframeY >= 6)
            {
                explodeframeY = 0;
                explodepoints.clear();
            }
        }
    }

    if (page >= 2)
    {
        HFONT hfont = CreateFont(70, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움"));
        HFONT holdfont = (HFONT)SelectObject(mem1dc, hfont);
        if (player.getLife() <= 0)
        {
            FillRect(mem1dc, &rectView, GetSysColorBrush(COLOR_WINDOW + 2));
            Font font(L"Arial", 60, FontStyleBold, UnitPixel);
            StringFormat format;
            format.SetAlignment(StringAlignmentCenter);
            format.SetLineAlignment(StringAlignmentCenter);

            SolidBrush sbrush(Color::OrangeRed);
            g.DrawString(L"Game Over", _tcslen(L"Game Over"), &font, RectF(0, 0, rectView.right, rectView.bottom), &format, &sbrush);
        }
        else
        {
            rect.X = 0;
            rect.Y = 0;
            rect.Width = rectView.right;
            rect.Height = rectView.bottom;
            g.FillRectangle(myBrush, rect);
            Font font(L"Arial", 60, FontStyleBold, UnitPixel);
            StringFormat format;
            format.SetAlignment(StringAlignmentCenter);
            format.SetLineAlignment(StringAlignmentCenter);
            if (fontcolor == 0)
            {
                SolidBrush sbrush(Color::Blue);
                g.DrawString(L"Game Clear", _tcslen(L"Game Clear"), &font, RectF(0, 0, rectView.right, rectView.bottom), &format, &sbrush);
            }
            else
            {
                SolidBrush sbrush(Color::Red);
                g.DrawString(L"Game Clear", _tcslen(L"Game Clear"), &font, RectF(0, 0, rectView.right, rectView.bottom), &format, &sbrush);
            }
        }
    }

    if (page == 0)
    {
        FillRect(mem1dc, &rectView, GetSysColorBrush(COLOR_WINDOW));
        HFONT hfont = CreateFont(70, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움"));
        HFONT holdfont = (HFONT)SelectObject(mem1dc, hfont);
        TextOut(mem1dc, 400, 300, L"갈스패닉 모작", _tcslen(L"갈스패닉 모작"));
        hfont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("돋움"));
        holdfont = (HFONT)SelectObject(mem1dc, hfont);
        TextOut(mem1dc, 400, 450, L"Press 's' key to start", _tcslen(L"Press 's' key to start"));
    }

    BitBlt(hdc, 0, 0, rectView.right, rectView.bottom, mem1dc, 0, 0, SRCCOPY);
    //GdiTransparentBlt(hdc, 0, 0, rectView.right, rectView.bottom, mem1dc,
        //0, 0, rectView.right, rectView.bottom, RGB(255, 255, 255));
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
