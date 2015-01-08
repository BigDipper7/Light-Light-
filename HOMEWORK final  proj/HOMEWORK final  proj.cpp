/**
* �����B
* 1252937
* 13122359969
* xianyuhuanyue@gmail.com
* 
* �������ļ���ֻ������ļ����ţ�û�зָ��ͷ�ļ�
* ���������е��߼�
**/
// HOMEWORK final  proj.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "HOMEWORK final  proj.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

using namespace std;

//===================define============================================================================================================
#define MAX_LOADSTRING 100
//===================end define========================================================================================================


//===================variables=========================================================================================================
int len = 4; //len MUST > 3
const int width = 60;//���ӵĿ��
const int startX=50,startY=50;
const int maxLEN = 10;
bool needAutoComplete = false;
const int autoCpltLock = 17;//�Զ���ɵ����� Ҳ����˵�����Ƕ�����Żᴥ��һ�����ֵ��Զ�����¼�
int autoCpltLable = 0;//��Ӱ���

bool map[maxLEN][maxLEN]= {
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true}
			}, 
	input[maxLEN][maxLEN] = {
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true}
			},
	curCvs[maxLEN][maxLEN]= {
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true},
				{true,true,true,true,true,true,true,true,true,true}
			};
//
//bool map[len][len]= {
//				{true,true,true},
//				{true,true,true},
//				{true,true,true}
//			}, 
//	input[len][len]= {
//				{true,true,true},
//				{true,true,true},
//				{true,true,true}
//			}, 
//	curCvs[len][len]= {
//				{true,true,true},
//				{true,true,true},
//				{true,true,true}
//			};


int R,G,B;
int status;
bool isMosInRegion = false;
//===================end variables=====================================================================================================


// =====================functions===========================================================================================

//Log...
template<int LEN>
void logMapContain(bool map[][LEN], int LEN) 
{
	for(int i=0; i<LEN;i++)
	{
		for(int j=0; j<LEN; j++)
		{
			string str = map[i][j]?"true\t":"false\t";
			Log(str);
		}
		cout<<endl;
	}
}

//Log....
void Log(const string str)
{
	//�����Ǵ�ӡ�����õ� �������û�µĻ���ע�͵�����
	/*if(AllocConsole())
	{
		freopen("CONOUT$","w",stdout);
		cout<<str;
	}else
	{
		cout<<str;
	}*/
}

//�ж�����λ��,����һ��POINT
POINT getMousePos(int clX, int clY, int LEN)
{
	POINT pt={clX, clY};
	if(clX<=startX || clY<=startY 
		|| clX>=startX+LEN*width || clY>=startY+LEN*width)
	{//�����߽磬ȫ����Ϊ������Խ��ģ�not in region
		Log("Out of bound... Not in focus regin...\n");
		isMosInRegion=false;
		return pt;
	}

	int m,n;
	n=(clX-startX)/width;
	m=(clY-startY)/width;
	string str;
	char temp[256];
	sprintf(temp,"m=%d, n=%d \n",m,n);
	str = temp;
	Log(str);
	pt.x = m;
	pt.y = n;
	isMosInRegion=true;
	return pt;
}

//���Ƹ���
void drawGrid(const int sx, const int sy, int LEN, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(0,0,0));
	SelectObject(hdc, hPen);
	//const int width = 50;//���ӵĿ��
	for(int i=0; i<LEN+1; i++)
	{
		int curX=sx, curY = sy+i*width;
		MoveToEx(hdc, curX, curY, NULL);
		LineTo(hdc, curX+LEN*width, curY);
	}
	for(int i=0; i<LEN+1; i++)
	{
		int curX=sx+i*width, curY = sy;
		MoveToEx(hdc, curX, curY, NULL);
		LineTo(hdc, curX, curY+LEN*width);
	}
	DeleteObject(hPen);
}

//����������
template<int LEN>
void fillGrid(const int sx, const int sy, HDC hdc, bool map[][LEN], bool curCvs[][LEN], int LEN) 
{
	HBRUSH hBrush;
	RECT rect;
	int padding = 3;
	//const int width = 50;//���ӵĿ��
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			int curX = sx + j*width, curY = sy + i*width;

			if(!map[i][j])
			{
				//Ҳ���Ǹ�Ϳ�ڵĵط�
				SetRect(&rect, curX+padding, curY+padding, curX+width-padding, curY+width-padding);
				hBrush = CreateSolidBrush(RGB(195,195,195));
				FillRect(hdc,&rect,hBrush);
			}else
			{
				if(curCvs[i][j])
				{//true-> highlight
					hBrush = CreateSolidBrush(RGB(0,157,255));
				}else
				{
					hBrush = CreateSolidBrush(RGB(0,82,117));
				}
				HPEN hPen = CreatePen(PS_NULL,0,RGB(255,255,25));
				SelectObject(hdc, hBrush);
				SelectObject(hdc, hPen);
				Ellipse(hdc, curX+padding, curY+padding, curX+width-padding, curY+width-padding);
				DeleteObject(hBrush);
				DeleteObject(hPen);
			}
		}
	}
	/*SetRect(&rect, sx+padding, sy+padding, sx+width-padding, sy+width-padding);
	FillRect(hdc,&rect,hBrush);*/

	DeleteObject(hBrush);
}

//draw current frame,���Ƶ�ǰ��һ����ͼ���������Ӻ����е���������Ҳ���ǻ�ȫ������һ�飬
//      ���Ǳ������ĳ���ط��Ķ���������Ҫȫ�����ƣ����ܵ������������������Ч�ʱȽϵ͵�
template<int LEN>
void drawCurFrame(bool curCvs[][LEN], bool map[][LEN], int LEN, HDC hdc)
{
	//�ȸ���map[][]���Ƴ������ͼ�� �������Ϊtrue�����Բ�� Ϊfalse����䷽��Ϊ��ɫ

	int sx = startX, sy = startY;//ͼ������ ���Ͻ�
	
	drawGrid(sx, sy, LEN, hdc);
	fillGrid(sx, sy, hdc, map, curCvs, LEN);

	//End...
}

//draw current frame,���Ƶ�ǰ��һ����ͼ���������Ӻ����е���������Ҳ���ǻ�ȫ������һ�飬
//      ���Ǳ������ĳ���ط��Ķ���������Ҫȫ�����ƣ����ܵ������������������Ч�ʱȽϵ͵�
//���������Ҫ���ڳ�ʼ���������ֵ�ʱ��Ҫ�õ�
template<int LEN>
void drawInitCurFrame(bool curCvs[][LEN], bool map[][LEN], bool input[][LEN], int LEN, HWND hWnd, HDC hdc)
{
	initData(curCvs,input,map,len);//init data...
	refreshScreen(hWnd, hdc);//draw screen white
	drawCurFrame(curCvs,map,len,hdc);//draw current canvas
}

//judge weather is all complete, that means you are WIN......
template<int LEN>
bool isComplete(bool curCvs[][LEN], bool map[][LEN], int LEN)
{
	//���е�ͼ�����Լ�����ж�
	//���Ƚ����ж�
	bool isComplete = true;
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(map[i][j])//���ȱ�֤�ǿ��Դ���������Ҳ���ǻ��Ƴ����ĵ���
			{
				if(!curCvs[i][j])
				{
					isComplete = false;
					break;
				}
			}
		}
	}
	return isComplete;
}

//generate/update values in curCvs[][] After You Input initValues... 
//:  normally be invoked when you generate a new game , in the beginning
template<int LEN>
void genInitCanvas(bool curCvs[][LEN],const bool input[][LEN],const bool map[][LEN], int LEN)
{
	//����input[][] & map[][] : every element....
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(input[i][j])
			{
				//���� ���� ˢ���������µĵ�ͼ��ֻ�������
				getMtxAftInpOneVal(curCvs,map,LEN,i,j);
			}
		}
	}
}

//generate input[][] and map[][],notice that map should have at least ?? percent of all cubes
template<int LEN>
void genDefultValues(bool curCvs[][LEN], bool input[][LEN], bool map[][LEN], int LEN) {

	for(int i=0; i<LEN; i++) {
		for(int j=0; j<	LEN; j++) {
			curCvs[i][j] = true;  		
		}
	}

	for(int i=0; i<LEN; i++) {
		for(int j=0; j<	LEN; j++) {
			input[i][j] = getRandomValue(5,3);  		
		}
	}
	
	for(int i=0; i<LEN; i++) {
		for(int j=0; j<	LEN; j++) {
			map[i][j] = getRandomValue(100,84); 
		}
	}
	
}

//generate a random value
bool getRandomValue(int maxVal,int percent) {
	int r = rand()%maxVal+1;

	string str;
	char temp[256];
	sprintf(temp,"rand value: %d\n",r);
	str = temp;
	Log(str);

	if(r < percent)
		return true;   

	return false;
}

//generate init thing
template<int LEN>
void initData(bool curCvs[][LEN], bool input[][LEN], bool map[][LEN], int LEN)
{
	genDefultValues(curCvs,input,map,len); 
	logMapContain(input,len);
	genInitCanvas(curCvs, input, map, len);
}

//refresh screen to white......
void refreshScreen(HWND hWnd,HDC hdc)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,255));
	FillRect(hdc,&rect,hBrush);
}

/**
* @Deprecated
* I WON'T USE IT
**/
// draw the sence if you input a bool[][] matrix 
template<int LEN>
void drawCurCanvas(bool curCvs[][LEN], bool input[][LEN], bool map[][LEN], int LEN)
{
	bool tempMtx[LEN][LEN];
	//����input[][] & map[][] : every element....
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(input[i][j])
			{
				//���� ���� ˢ���������µĵ�ͼ��ֻ�������
				getMtxAftInpOneVal(curCvs,map,LEN,i,j);
			}
		}
	}
	////��������µĵ�ͼ
	////���е�ͼ�����Լ�����ж�
	////���Ƚ����ж�
	//bool isComplete = true;
	//for(int i=0; i<LEN; i++)
	//{
	//	for(int j=0; j<LEN; j++)
	//	{
	//		if(map[i][j])//���ȱ�֤�ǿ��Դ���������Ҳ���ǻ��Ƴ����ĵ���
	//		{
	//			if(!curCvs[i][j])
	//			{
	//				isComplete = false;
	//				break;
	//			}
	//		}
	//	}
	//}
	//if(isComplete)
	//	//all done

}



// get result if you input only one value : ˢ�µ�ǰ״̬��Ҳ����˵���������һ��ֵ�����꣬Ȼ��ˢ�µ�ǰ��curCvs��״̬
template<int LEN>
void getMtxAftInpOneVal(bool curCvs[][LEN],const bool map[][LEN], const int LEN, const int m, const int n)
{
	if(LEN<0 || m<0 || m>=LEN || n<0 || n>=LEN)
	{
		Log("invalide parameters\n");
		return;
	}

	if(!map[m][n])
	{
		Log("map[][] is false... , can't change\n");
		return;
	}
	/*
	*	if exist.... the following are the postion relationship
	*
	*					curCvs[m-1][n]
	* curCvs[m][n-1]	curCvs[m][n]	curCvs[m][n+1]
	*					curCvs[m+1][n]
	*
	*/
	bool u = false, d = false, l = false, r = false;
	switch(getCaseCode(m,n,LEN))
	{
	case CASE_UDLR:
		Log("CASE_UDLR\n");
		u=true; d=true; l=true; r=true; 
		break;
	case CASE_DR:
		Log("CASE_DR\n");
		d=true; r=true;
		break;
	case CASE_DLR:
		Log("CASE_DLR\n");
		d=true; l=true; r=true;
		break;
	case CASE_DL:
		Log("CASE_DL\n");
		d=true; l=true;
		break;
	case CASE_UDR:
		Log("CASE_UDR\n");
		u=true; d=true; r=true;
		break;
	case CASE_UDL:
		Log("CASE_UDL\n");
		u=true; d=true; l=true;
		break;
	case CASE_UR:
		Log("CASE_UR\n");
		u=true; r=true;
		break;
	case CASE_ULR:
		Log("CASE_ULR\n");
		u=true; l=true; r=true;
		break;
	case CASE_UL:
		Log("CASE_UL\n");
		u=true; l=true;
		break;
	case CASE_NULL:
		Log("======Something wrong in getCaseMode()======= \n");
		return;
	}

	/*
	*	if exist.... the following are the postion relationship
	*
	*					curCvs[m-1][n]
	* curCvs[m][n-1]	curCvs[m][n]	curCvs[m][n+1]
	*					curCvs[m+1][n]
	*
	*/
	//�ֱ�����жϲ���ȡ��
	if(u)
	{
		if(map[m-1][n])
		{
			//ȡ��
			curCvs[m-1][n] = !curCvs[m-1][n];
		}
	}
	if(d)
	{
		if(map[m+1][n])
		{
			//ȡ��
			curCvs[m+1][n] = !curCvs[m+1][n];
		}
	}
	if(l)
	{
		if(map[m][n-1])
		{
			//ȡ��
			curCvs[m][n-1] = !curCvs[m][n-1];
		}
	}
	if(r)
	{
		if(map[m][n+1])
		{
			//ȡ��
			curCvs[m][n+1] = !curCvs[m][n+1];
		}

	}
	curCvs[m][n]=!curCvs[m][n];//����Ҫ����ֱ�ӽ���ȡ������
	//input[m][n]=!input[m][n];//ֱ�ӽ���ȡ���������Ļ��Ϳ���ʵʱ������µ�inputֵ

}
//get case: Case Code
//�ܹ���֤�����m��n�ǿ϶��ڵ���ڿ�����Ӧ��������
int getCaseCode(const int m, const int n, const int LEN)
{
	int max = LEN-1;
	if(m!=0 && m!=max
		&& n!=0 && n!=max)
	{
		return CASE_UDLR;
	}
	else if(m==0)
	{
		if(n!=0 && n!=max)
		{
			return CASE_DLR;
		}
		else if(n == 0)
		{
			return CASE_DR;
		}
		else if(n == max)
		{
			return CASE_DL;
		}
	}
	else if(m == max)
	{
		if(n!=0 && n!=max)
		{
			return CASE_ULR;
		}
		else if(n == 0)
		{
			return CASE_UR;
		}
		else if(n == max)
		{
			return CASE_UL;
		}
	}
	else if(m!=0 && m!=max)
	{
		if(n == 0)
		{
			return CASE_UDR;
		}
		else if(n == max)
		{
			return CASE_UDL;
		}
	}
	return CASE_NULL;
}


// ����Ч��
// �����ڡ������Ϣ�������
INT_PTR CALLBACK Finish(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hDlg, LOWORD(wParam));
			status = STATUS_BEGIN;
			//initData(curCvs, input, map, len);
			return (INT_PTR)TRUE;
		}
		else if(LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			ExitProcess(0);
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
// =====================functions ends===========================================================================================



// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������


// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HOMEWORKFINALPROJ, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMEWORKFINALPROJ));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORKFINALPROJ));
	//wcex.hIcon			= (HICON)LoadImage(NULL,TEXT("icon.ico"),IMAGE_ICON,0,0,LR_LOADFROMFILE);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HOMEWORKFINALPROJ);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			srand( (unsigned)time(NULL));
			SetTimer(hWnd, TIMER_ID, 1000/60, NULL); //һ����ʮ֡
			status = STATUS_BEGIN;
			initData(curCvs, input, map, len);

			PlaySound(TEXT("BGM\\bg.wav"), NULL, SND_FILENAME | SND_ASYNC);

		}
		break;

	case WM_TIMER:
		{
			hdc = GetDC(hWnd);

			//��ʾ��ǰʱ�� ��ʼ���ṹ��
			SYSTEMTIME time;
			GetLocalTime(&time);
			int sec = time.wSecond;
			int min = time.wMinute;
			int hor = time.wHour;
			int day = time.wDay;
			int mot = time.wMonth;
			int yea = time.wYear;

			//��ʾ��ǰʱ��
			wchar_t buf[256];
			wsprintf(buf, TEXT("%d �� %d �� %d �� %d ʱ %d �� %d ��"), yea, mot, day, hor, min, sec);
			SetWindowText(hWnd, buf);

			bool needBreak=false;
			if(needAutoComplete)
			{
				autoCpltLable++;
				if(autoCpltLable <= autoCpltLock)
					break;
				autoCpltLable=0;
				//����input[][]�������Զ����
				for(int i=0;i<len;i++)
				{
					for(int j=0; j<len; j++)
					{
						if(input[i][j])
						{
							input[i][j]=!input[i][j];//������仰 ��Ӧ�÷ŵ�����ȥ ��ΪֻҪ����Ϊtrue�������Ҫ������ˢ�³�false ��������Ƿ���Ҫ�ػ� ��Ҫ����һ��
								
							if(map[i][j])//����жϺ������� ��Ϊ��������жϲ��ܹ���֤���Լ��������ʱ������ݲ�����Ϊ��������inputΪtrue����mapΪfalse�������Ȼ���ֱ�ӷ����˳�������������������ģ������ʱ�䲢�����ȶ���ÿ��������ͻ���һ���ػ����� �������ĺܱ�Ҫ��һ���ж�
							{
								int m = i;
								int n = j;
								getMtxAftInpOneVal(curCvs,map,len,m,n);
								drawCurFrame(curCvs,map,len,hdc);
								//input[m][n]=!input[m][n];//IMPORTANT
								logMapContain(input,len);
								if(isComplete(curCvs,map, len))
								{
									status = STATUS_COMPLETE;
									DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGFINISH), hWnd, Finish);
									Log("after end dialog\n");
									drawInitCurFrame(curCvs, map, input, len, hWnd, hdc);
								}
								needBreak=true;
								break;
							}
						}else
						{
							if(i==len-1 && j==len-1)
								needAutoComplete = false ;
						}
					}
					if(needBreak)
						break;
				}
			}
			

		}
		ReleaseDC(hWnd,hdc);
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		hdc = GetDC(hWnd);
		if(needAutoComplete)
			break;
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_RESTART:
			status = STATUS_RESTART;
			break;
		case IDM_AUTOCOMLETE:
			needAutoComplete=true;
			break;
		case IDM_3:
			len=3;
			status = STATUS_RESTART;
			//initData(curCvs,input,map,len);//init data...
			//refreshScreen(hWnd, hdc);//draw screen white
			//drawCurFrame(curCvs,map,len,hdc);//draw current canvas
			break;
		case IDM_4:
			len=4;
			status = STATUS_RESTART;
			break;
		case IDM_5:
			len=5;
			status = STATUS_RESTART;
			break;
		case IDM_6:
			len=6;
			status = STATUS_RESTART;
			break;
		case IDM_7:
			len=7;
			status = STATUS_RESTART;
			break;
		case IDM_8:
			len=8;
			status = STATUS_RESTART;
			break;
		case IDM_9:
			len=9;
			status = STATUS_RESTART;
			break;
		case IDM_10:
			len=10;
			status = STATUS_RESTART;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		if(status == STATUS_RESTART)
		{
			drawInitCurFrame(curCvs, map, input, len, hWnd, hdc);
			status = STATUS_BEGIN;
		}
		ReleaseDC(hWnd, hdc);
		break;

	case WM_LBUTTONDOWN:
		{
			hdc = GetDC(hWnd);
			
			if(status == STATUS_BEGIN) {
				int clX = LOWORD(lParam);
				int clY = HIWORD(lParam);

				POINT pt = getMousePos(clX,clY,len);
				if(isMosInRegion)
				{
					int m = pt.x;
					int n = pt.y;
					getMtxAftInpOneVal(curCvs,map,len,m,n);
					input[m][n]=!input[m][n];
					drawCurFrame(curCvs,map,len,hdc);
					logMapContain(input,len);
					if(isComplete(curCvs,map, len))
					{
						status = STATUS_COMPLETE;
						DialogBox(hInst, MAKEINTRESOURCE(IDD_DLGFINISH), hWnd, Finish);
						Log("after end dialog\n");
						drawInitCurFrame(curCvs, map, input, len, hWnd, hdc);/*
						refreshScreen(hWnd, hdc);
						drawCurFrame(curCvs,map,len,hdc);*/

					}

				}
			}
			ReleaseDC(hWnd, hdc);
		}
		break;

	case WM_KEYDOWN:
		{
			char te[256];
			string str;
			sprintf(te,"you type button %d\n",wParam);
			str = te;
			Log(str);

			if(needAutoComplete)//ֱ���˳�
				break;

			hdc = GetDC(hWnd);

			switch(wParam)
			{
			case 116://F5 refresh
				drawInitCurFrame(curCvs, map, input, len, hWnd, hdc);
				status = STATUS_BEGIN;
				break;
			case 112://F1 autocomplete
				needAutoComplete = true;
				break;
			}

			ReleaseDC(hWnd,hdc);
		}
		break;

	case WM_PAINT:
		{
			Log("WM_PAINT\n");

			hdc = BeginPaint(hWnd, &ps);
			// TODO: �ڴ���������ͼ����...
			drawCurFrame(curCvs,map,len,hdc);

			//bool mp[len][len] = {
			//	{true,true,true,false,true},
			//	{true,true,false,true,true},
			//	{true,false,false,false,true},
			//	{true,true,false,false,true},
			//	{true,false,true,false,true},
			//};
			//bool curCus[len][len] = {
			//	{true,false,true,true,true},
			//	{true,true,true,true,true},
			//	{true,false,false,false,true},
			//	{true,false,true,true,true},
			//	{true,false,true,true,false}
			//};
			
			/*drawGrid(100,100,5,hdc);
			fillGrid(100,100,hdc,mp,curCus,5);*/

			//drawCurFrame(curCus, mp, len, hdc);

			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, TIMER_ID);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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

