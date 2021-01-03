
// GoBang.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "GoBang.h"
#include "GoBangDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <vector>


// CGoBangApp

BEGIN_MESSAGE_MAP(CGoBangApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGoBangApp 构造

CGoBangApp::CGoBangApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CGoBangApp 对象

CGoBangApp theApp;


// CGoBangApp 初始化

BOOL CGoBangApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			ChessBoard[i][j] = -1;
		}
	}//初始化棋盘
	CGoBangDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
int GetChessBoardColor(int nx, int ny)
{
	int color = ChessBoard[ny][nx];
	return color;
}
int GetChessCount(int nx, int ny, CPoint* begin = NULL, CPoint* end = NULL)//获取指定棋子各个方向的同色棋子个数最大值
{
	int color = GetChessBoardColor(nx, ny);
	if (color == -1)
		return -1;

	int x = nx, y = ny;
	int m_max, count;
	while (--y >= 0 && GetChessBoardColor(x, y) == color);
	y++;
	for (count = 1; (++y < SIZE) && (GetChessBoardColor(x, y) == color); count++);
	m_max = count;
	//y轴
	x = nx, y = ny;
	while (--x >= 0 && GetChessBoardColor(x, y) == color);
	x++;
	for (count = 1; ++x < SIZE && GetChessBoardColor(x, y) == color; count++);
	if (m_max < count)
		m_max = count;
	//x轴
	x = nx, y = ny;
	while (x - 1 >= 0 && y - 1 >= 0 && GetChessBoardColor(x - 1, y - 1) == color)
		x--, y--;
	for (count = 1; x + 1 < SIZE && y + 1 < SIZE && GetChessBoardColor(x + 1, y + 1) == color; count++)
		x++, y++;
	if (m_max < count)
		m_max = count;
	//左下到右上
	x = nx, y = ny;
	while (x - 1 >= 0 && y + 1 < SIZE && GetChessBoardColor(x - 1, y + 1) == color)
		x--, y++;
	for (count = 1; x + 1 < SIZE && y - 1 >= 0 && GetChessBoardColor(x + 1, y - 1) == color; count++)
		x++, y--;
	if (m_max < count)
		m_max = count;
	//左上到右下
	return m_max;
}
int GetWinner()//获取赢家，-1无,0白,1黑
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			int color = GetChessBoardColor(i, j);
			if (color != -1)
			{
				if (GetChessCount(i, j) >= 5)
					return color;
			}
		}
	}
	return -1;
}

int GetResult(int nx, int ny)
{
	CPoint pt[2];
	int u;
	int color = GetChessBoardColor(nx, ny);
	if (color == -1)
		return -1;

	int x = nx, y = ny;
	int m_max, count;
	int t[2];
	while (--y >= 0 && GetChessBoardColor(x, y) == color);
	y++;
	pt[0].SetPoint(x, y);
	for (count = 1; (++y < SIZE) && (GetChessBoardColor(x, y) == color); count++);
	pt[1].SetPoint(x, y);
	u = 1;
	m_max = count;
	x = nx, y = ny;

	while (--x >= 0 && GetChessBoardColor(x, y) == color);
	x++;
	t[0] = x;
	for (count = 1; ++x < SIZE && GetChessBoardColor(x, y) == color; count++);
	if (m_max < count) 
	{
		m_max = count;
		pt[0].SetPoint(t[0], y);
		pt[1].SetPoint(x, y);
		u = 0;
	}
	x = nx, y = ny;

	while (x - 1 >= 0 && y - 1 >= 0 && GetChessBoardColor(x - 1, y - 1) == color)
		x--, y--;
	t[0] = x;
	t[1] = y;
	for (count = 1; x + 1 < SIZE && y + 1 < SIZE && GetChessBoardColor(x + 1, y + 1) == color; count++)
		x++, y++; 
	if (m_max < count)
	{
		m_max = count;
		pt[0].SetPoint(t[0], t[1]);
		pt[1].SetPoint(x, y);
		u = 2;
	}
	x = nx, y = ny;

	while (x - 1 >= 0 && y + 1 < SIZE && GetChessBoardColor(x - 1, y + 1) == color)
		x--, y++;
	t[0] = x;
	t[1] = y;
	for (count = 1; x + 1 < SIZE && y - 1 >= 0 && GetChessBoardColor(x + 1, y - 1) == color; count++)
		x++, y--;
	if (m_max < count)
	{
		m_max = count;
		pt[0].SetPoint(t[0], t[1]);
		pt[1].SetPoint(x, y);
		u = 3;
	}
	x = nx, y = ny;

	int result;
	switch (u)
	{
	case 0:
		
		break;
	}
}
