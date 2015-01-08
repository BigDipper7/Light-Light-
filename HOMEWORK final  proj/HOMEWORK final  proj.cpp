/**
* 许铭淏
* 1252937
* 13122359969
* xianyuhuanyue@gmail.com
* 
* 是主体文件，只有这个文件，嗯，没有分割成头文件
* 处理了所有的逻辑
**/
// HOMEWORK final  proj.cpp : 定义应用程序的入口点。
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
const int width = 60;//格子的宽度
const int startX=50,startY=50;
const int maxLEN = 10;
bool needAutoComplete = false;
const int autoCpltLock = 17;//自动完成的上限 也就是说究竟是多少秒才会触发一次这种的自动完成事件
int autoCpltLable = 0;//添加按键

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
	//下面是打印调试用的 但是如果没事的话就注释掉好了
	/*if(AllocConsole())
	{
		freopen("CONOUT$","w",stdout);
		cout<<str;
	}else
	{
		cout<<str;
	}*/
}

//判断鼠标的位置,返回一个POINT
POINT getMousePos(int clX, int clY, int LEN)
{
	POINT pt={clX, clY};
	if(clX<=startX || clY<=startY 
		|| clX>=startX+LEN*width || clY>=startY+LEN*width)
	{//包括边界，全部认为我们是越界的，not in region
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

//绘制格子
void drawGrid(const int sx, const int sy, int LEN, HDC hdc)
{
	HPEN hPen = CreatePen(PS_SOLID,1,RGB(0,0,0));
	SelectObject(hdc, hPen);
	//const int width = 50;//格子的宽度
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

//填充格子内容
template<int LEN>
void fillGrid(const int sx, const int sy, HDC hdc, bool map[][LEN], bool curCvs[][LEN], int LEN) 
{
	HBRUSH hBrush;
	RECT rect;
	int padding = 3;
	//const int width = 50;//格子的宽度
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			int curX = sx + j*width, curY = sy + i*width;

			if(!map[i][j])
			{
				//也就是该涂黑的地方
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

//draw current frame,绘制当前的一个地图，包括格子和所有的填充物，所以也就是会全部绘制一遍，
//      但是比如绘制某个地方的东西，不需要全部绘制，则不能调用这个方法，否则是效率比较低的
template<int LEN>
void drawCurFrame(bool curCvs[][LEN], bool map[][LEN], int LEN, HDC hdc)
{
	//先根据map[][]绘制出这个地图来 就是如果为true则绘制圆形 为false则填充方块为灰色

	int sx = startX, sy = startY;//图像的起点 左上角
	
	drawGrid(sx, sy, LEN, hdc);
	fillGrid(sx, sy, hdc, map, curCvs, LEN);

	//End...
}

//draw current frame,绘制当前的一个地图，包括格子和所有的填充物，所以也就是会全部绘制一遍，
//      但是比如绘制某个地方的东西，不需要全部绘制，则不能调用这个方法，否则是效率比较低的
//这个方法主要是在初始化整个布局的时候要用到
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
	//进行地图绘制以及结果判断
	//首先进行判断
	bool isComplete = true;
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(map[i][j])//首先保证是可以触碰地区，也就是绘制出来的地区
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
	//根据input[][] & map[][] : every element....
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(input[i][j])
			{
				//两次 遍历 刷新了最后的新的地图，只差绘制了
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
	//根据input[][] & map[][] : every element....
	for(int i=0; i<LEN; i++)
	{
		for(int j=0; j<LEN; j++)
		{
			if(input[i][j])
			{
				//两次 遍历 刷新了最后的新的地图，只差绘制了
				getMtxAftInpOneVal(curCvs,map,LEN,i,j);
			}
		}
	}
	////获得了最新的地图
	////进行地图绘制以及结果判断
	////首先进行判断
	//bool isComplete = true;
	//for(int i=0; i<LEN; i++)
	//{
	//	for(int j=0; j<LEN; j++)
	//	{
	//		if(map[i][j])//首先保证是可以触碰地区，也就是绘制出来的地区
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



// get result if you input only one value : 刷新当前状态，也就是说根据输入的一个值的坐标，然后刷新当前的curCvs的状态
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
	//分别进行判断并且取反
	if(u)
	{
		if(map[m-1][n])
		{
			//取反
			curCvs[m-1][n] = !curCvs[m-1][n];
		}
	}
	if(d)
	{
		if(map[m+1][n])
		{
			//取反
			curCvs[m+1][n] = !curCvs[m+1][n];
		}
	}
	if(l)
	{
		if(map[m][n-1])
		{
			//取反
			curCvs[m][n-1] = !curCvs[m][n-1];
		}
	}
	if(r)
	{
		if(map[m][n+1])
		{
			//取反
			curCvs[m][n+1] = !curCvs[m][n+1];
		}

	}
	curCvs[m][n]=!curCvs[m][n];//不需要条件直接进行取反操作
	//input[m][n]=!input[m][n];//直接进行取反，这样的话就可以实时获得最新的input值

}
//get case: Case Code
//能够保证输入的m和n是肯定在点击在可以相应的区域内
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


// 触发效果
// “关于”框的消息处理程序。
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



// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名


// 此代码模块中包含的函数的前向声明:
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

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_HOMEWORKFINALPROJ, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMEWORKFINALPROJ));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
			SetTimer(hWnd, TIMER_ID, 1000/60, NULL); //一秒六十帧
			status = STATUS_BEGIN;
			initData(curCvs, input, map, len);

			PlaySound(TEXT("BGM\\bg.wav"), NULL, SND_FILENAME | SND_ASYNC);

		}
		break;

	case WM_TIMER:
		{
			hdc = GetDC(hWnd);

			//显示当前时间 初始化结构体
			SYSTEMTIME time;
			GetLocalTime(&time);
			int sec = time.wSecond;
			int min = time.wMinute;
			int hor = time.wHour;
			int day = time.wDay;
			int mot = time.wMonth;
			int yea = time.wYear;

			//显示当前时间
			wchar_t buf[256];
			wsprintf(buf, TEXT("%d 年 %d 月 %d 日 %d 时 %d 分 %d 秒"), yea, mot, day, hor, min, sec);
			SetWindowText(hWnd, buf);

			bool needBreak=false;
			if(needAutoComplete)
			{
				autoCpltLable++;
				if(autoCpltLable <= autoCpltLock)
					break;
				autoCpltLable=0;
				//遍历input[][]来进行自动完成
				for(int i=0;i<len;i++)
				{
					for(int j=0; j<len; j++)
					{
						if(input[i][j])
						{
							input[i][j]=!input[i][j];//但是这句话 不应该放到里面去 因为只要出现为true的情况就要把数据刷新成false 不管最后是否需要重绘 都要做这一步
								
							if(map[i][j])//这个判断很有意义 因为有了这个判断才能够保证你自己的输入的时候的数据不会因为出现那种input为true但是map为false的情况，然后就直接反向退出的情况，这样会让你的模拟点击的时间并不是稳定的每隔多少秒就会有一回重绘的情况 这个是真的很必要的一部判断
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
		// 分析菜单选择:
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

			if(needAutoComplete)//直接退出
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
			// TODO: 在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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

