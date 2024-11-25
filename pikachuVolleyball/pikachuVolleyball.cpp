// pikachuVolleyball.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
//#include "Glober.h"
//#include "Player.h"
#include "framework.h"
#include "pikachuVolleyball.h"


#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

HBITMAP ballBitmap = NULL;
HBITMAP p1Bitmap = NULL;
HBITMAP p2Bitmap = NULL;
//

RECT g_homeGround = { 0,0,0,0 };  //homeGround 전역변수를 통해 피카츄1, 2가 네트나 창 밖으로 이동 못하게 만들었음
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
	LoadStringW(hInstance, IDC_PIKACHUVOLLEYBALL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 애플리케이션 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PIKACHUVOLLEYBALL));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PIKACHUVOLLEYBALL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PIKACHUVOLLEYBALL);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.


	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1600, 1000, nullptr, nullptr, hInstance, nullptr);  //1600, 1000 초기 해상도 크기 조정

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;
}

void ImageView(HINSTANCE hInstance) {
ballBitmap = (HBITMAP)LoadImage(NULL,
	L"..\..\..\Desktop\Debug\ball.bmp",
	IMAGE_BITMAP,
	0, 0,
	LR_LOADFROMFILE | LR_DEFAULTSIZE);
p1Bitmap = (HBITMAP)LoadImage(NULL,
	L"..\..\..\Desktop\Debug\p1.bmp",
	IMAGE_BITMAP,
	0, 0,
	LR_LOADFROMFILE | LR_DEFAULTSIZE);
p2Bitmap = (HBITMAP)LoadImage(NULL,
	L"..\..\..\Desktop\Debug\p2.bmp",
	IMAGE_BITMAP,
	0, 0,
	LR_LOADFROMFILE | LR_DEFAULTSIZE);


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

RECT p1, p2;    //플레이어 전역 변수
RECT ball, net;      //배구공 전역 변수
bool perpendicular = true;      //true일때 공이 아래로 내려감
int BallSpeed = 20;
TCHAR p1_buffer[256];
TCHAR p2_buffer[256];
int ball_x = 0, ball_y = 1;
RECT clash;					//객체 겹침 RECT 변수
int p1_score = 0;			//p1 점수
int p2_score = 0;			//p2 점수
int p1_StartPoint = 320;		//p1 서브
int p2_StartPoint = 1220;		//p2 서브
int Timer = 5;			//sub권이 바뀔 때 공이 커지면서 플레이어와 InterSectRect가 일어나버림

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	HDC hdc = GetDC(hWnd);
	switch (message)
	{
	case WM_TIMER: {    //case WM_KEYDOWN으로 할 시 플레이어1,2가 동시에 움직이지 못함
		// TIMER로 대략 60프레임으로 계속 호출을 해서 동시에 KeyDown 2개 이상 기능



		_stprintf_s(p1_buffer, _T("%d"), p1_score);
		_stprintf_s(p2_buffer, _T("%d"), p2_score);

		//플레이어1 이동속도 A,D로 조작
		if (GetAsyncKeyState('A')) {   //유니코드 값으로 A를 입력시
			p1.left -= 15;
			p1.right -= 15;
			if (p1.left < g_homeGround.left) {
				p1.left = g_homeGround.left;
				p1.right = g_homeGround.left + 90;
			}
		}
		if (GetAsyncKeyState('D')) {
			p1.left += 15;
			p1.right += 15;
			if (p1.right > net.right) {
				p1.right = net.right;
				p1.left = net.right - 90;
			}
		}
		//플레이어2 이동속도 방향키 왼쪽 오른쪽으로 조작
		if (GetAsyncKeyState(VK_LEFT)) {
			p2.left -= 15;
			p2.right -= 15;
			if (p2.left < net.left) {
				p2.left = net.left;
				p2.right = net.left + 90;
			}
		}
		if (GetAsyncKeyState(VK_RIGHT)) {
			p2.left += 15;
			p2.right += 15;
			if (p2.right > g_homeGround.right) {
				p2.right = g_homeGround.right;
				p2.left = g_homeGround.right - 90;
			}
		}

		if (2 == wParam) {

			ball.top += ball_y * BallSpeed;
			ball.bottom = ball.top + 70;
			ball.right += ball_x * BallSpeed;
			ball.left = ball.right - 70;

			IntersectRect(&clash, &ball, &p1);

		}
	}
				 InvalidateRect(hWnd, NULL, TRUE);

				 if (3 == wParam) {
					 if (Timer <= 0) {
						 if (ball.left < g_homeGround.left) {		//공이 왼쪽 벽에 부딪혔을 때
							 ball_x *= -1;
						 }
						 if (ball.right > g_homeGround.right) {	//공이 오른쪽 벽에 부딪혔을 때
							 ball_x *= -1;
						 }

						 if (IntersectRect(&clash, &ball, &net)) {		//공이 네트에 닿았을 때
							 if (clash.top != 0) {
								 ball_x *= -1;
							 }
						 }
						 if (IntersectRect(&clash, &ball, &p1)) {
							 if ((ball.right + ball.left) / 2 - (p1.right + p1.left) / 2 <= 0) {	//좌상단 맞았을 때
								 ball_x = -1;
								 ball_y = -1;
							 }

							 else {								//우상단 맞았을 때
								 // x + y -;
								 ball_x = 1;
								 ball_y = -1;
							 }
						 }
						 if (IntersectRect(&clash, &ball, &p2)) {
							 if ((ball.right + ball.left) / 2 - (p2.right + p2.left) / 2 <= 0) {	//좌상단 맞았을 때
								 ball_x = -1;
								 ball_y = -1;
							 }
							 else {								//우상단 맞았을 때
								 // x + y -;
								 ball_x = 1;
								 ball_y = -1;
							 }
						 }

						 if (ball.top <= 250) {			//공이 천장에 맞았을 때
							 ball_y *= -1;
						 }
						 if (ball.bottom >= p1.bottom) {		//공이 바닥에 닿았을 때
							 if ((ball.right + ball.left) / 2 <= (g_homeGround.right + g_homeGround.left) / 2) {
								 ball.right = p2_StartPoint;
								 ball_x = 0;
								 ball_y = 1;
								 ball.top = 390;
								 Timer = 5;
								 p2_score++;		//p2 서브
							 }
							 else {
								 ball.right = p1_StartPoint;
								 ball_x = 0;
								 ball_y = 1;
								 ball.top = 390;
								 Timer = 5;
								 p1_score++;		//p1 서브
							 }
						 }
					 }
					 else {
						 Timer--;
					 }
				 }

				 break;
	case WM_SIZE:
	{
		g_homeGround.right = LOWORD(lParam);
		g_homeGround.bottom = HIWORD(lParam);
	}
	break;
	case WM_CREATE: {
		ImageView(hInst);
		

		//화면 해상도 1600x1000 (가로x세로)
		p1.left = 310;
		p1.top = 710;
		p1.right = 400;
		p1.bottom = 800;

		p2.left = 1210;
		p2.top = 710;
		p2.right = 1300;
		p2.bottom = 800;

		ball.left = 320;
		ball.top = 320;
		ball.right = 390;
		ball.bottom = 390;


		net.left = 793;
		net.top = 500;
		net.right = 794;
		net.bottom = 800;


		SetTimer(hWnd, 1, 60, NULL);
		SetTimer(hWnd, 2, 60, NULL);
		SetTimer(hWnd, 3, 60, NULL);
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC memDC = CreateCompatibleDC(hdc);
		RECT rect;
		GetClientRect(hWnd, &rect);
		HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
		HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);

		HDC hdcMem = CreateCompatibleDC(memDC);
		BITMAP bm;
		GetObject(ballBitmap, sizeof(BITMAP), &bm);
		BitBlt(memDC, 0, 0, rect.right,rect.bottom, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, ballBitmap);
		DeleteObject(memBitmap);
		DeleteDC(hdcMem);
		if (ballBitmap) {
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, ballBitmap);

			BITMAP bm;
			GetObject(ballBitmap, sizeof(BITMAP), &bm);

			BitBlt(hdc, 100, 100, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

			SelectObject(hdcMem, oldBitmap);
			DeleteDC(hdcMem);
		}
		else {
			TextOut(hdc, 100, 100, L"Failed to load bitmap.", 21);
		}

		//화면 해상도 1600x1000 (가로x세로)
		Rectangle(hdc, 0, 800, 1587, 943);  //땅(사각형) 객체     공과 Intersect 하기 위해 사각형으로 생성 (해상도가 정확한 값이 아니기에 테스트로 1587, 943 구함)
		Rectangle(hdc, net.left, net.top, net.right, net.bottom); //네트(사각형) 객체    공과 Intersect 하기 위해 사각형으로 생성
		Rectangle(hdc, p1.left, p1.top, p1.right, p1.bottom);  //플레이어1 객체
		Rectangle(hdc, p2.left, p2.top, p2.right, p2.bottom);  //플레이어1 객체
		Ellipse(hdc, ball.left, ball.top, ball.right, ball.bottom);       //배구공(원) 객체


		TextOut(hdc, 150, 0, p1_buffer, lstrlenW(p1_buffer));
		TextOut(hdc, 1500, 0, p2_buffer, lstrlenW(p2_buffer));

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		if (ballBitmap) DeleteObject(ballBitmap);
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
