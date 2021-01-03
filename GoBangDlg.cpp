
// GoBangDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GoBang.h"
#include "GoBangDlg.h"
#include "afxdialogex.h"
#include"resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int ChessBoard[SIZE][SIZE];

// CGoBangDlg 对话框



CGoBangDlg::CGoBangDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GOBANG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGoBangDlg::DoDataExchange(CDataExchange* pDX)
{
	IsPlaying = false;
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGoBangDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CGoBangDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_QUIT, &CGoBangDlg::OnBnClickedQuit)
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ENDGAME, &CGoBangDlg::OnBnClickedEndgame)
END_MESSAGE_MAP()


// CGoBangDlg 消息处理程序

BOOL CGoBangDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGoBangDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		RECT rect;
		GetClientRect(&rect);
		InvalidateRect(&rect,FALSE);//清空窗口
		CPaintDC dc(this);
		CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
		dc.SelectObject(pen);
		for (int i = 0; i < SIZE; i++)
		{
			dc.MoveTo(50, 50 + i * 50);
			dc.LineTo(750, 50 + i * 50);
		}//绘制棋盘横线
		for (int i = 0; i < SIZE; i++)
		{
			dc.MoveTo(50 + i * 50, 50);
			dc.LineTo(50 + i * 50, 750);
		}//绘制棋盘竖线
		for (int nx = 0; nx < SIZE; nx++)
		{
			for (int ny = 0; ny < SIZE; ny++)
			{

				int color = GetChessBoardColor(nx, ny);
				if (color == 0)//白棋
				{
					CBrush brush_w(RGB(255, 255, 255));
					const CPoint o(50 * nx + 50, 50 * ny + 50);//圆心
					dc.SelectObject(brush_w);
					dc.Ellipse(o.x - 15, o.y - 15, o.x + 15, o.y + 15);
				}
				else if (color == 1)//黑棋
				{
					CBrush brush_b(RGB(0, 0, 0));
					const CPoint o(50 * nx + 50, 50 * ny + 50);//圆心
					dc.SelectObject(brush_b);
					dc.Ellipse(o.x - 15, o.y - 15, o.x + 15, o.y + 15);
				}
			}
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGoBangDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CGoBangDlg::OnBnClickedStart()
{
	if (IsPlaying && MessageBoxW(L"确定要重玩吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	GetDlgItem(IDC_START)->SetWindowTextW(L"重玩");
	IsPlaying = true;
	NowColor = 1;//黑先
	GetDlgItem(IDC_ENDGAME)->EnableWindow(TRUE);
	CleanChessBoard();
}


void CGoBangDlg::OnBnClickedQuit()
{
	if (!IsPlaying || MessageBoxW(L"正在游戏中，确定要退出吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDYES)
		EndDialog(0);
}


void CGoBangDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!IsPlaying || point.x < 40 || point.x>760 || point.y < 40 || point.y>760)
		return;
	int x = (round(point.x / 50.0) - 1);
	int y = (round(point.y / 50.0) - 1);
	if (GetChessBoardColor(x, y) != -1)//如果已有棋子
		return;
	SetChessBoardColor(x, y, NowColor);
	NowColor = (!NowColor);
	SendMessage(WM_SETCURSOR);
	//放置棋子
	int winner = GetWinner();
	int count = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (ChessBoard[i][j] != -1)
				count++;
		}
	}
	if (winner != -1||count == SIZE * SIZE)
	{
		if (winner == 0)
			MessageBox(L"白棋胜利！", L"双人五子棋", MB_OK | MB_ICONINFORMATION);
		else if (winner == 1)
			MessageBox(L"黑棋胜利！", L"双人五子棋", MB_OK | MB_ICONINFORMATION);
		else
			MessageBox(L"平局！", L"双人五子棋", MB_OK | MB_ICONINFORMATION);
		EndGame();
		return;
	}
	//判断输赢
}

void CGoBangDlg::SetChessBoardColor(int nx, int ny, int color)
{
	ChessBoard[ny][nx] = color;
	CDC* dc = this->GetDC();
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	dc->SelectObject(pen);
	if (color == 0)//白棋
	{
		CBrush brush_w(RGB(255, 255, 255));
		const CPoint o(50 * nx + 50, 50 * ny + 50);//圆心
		dc->SelectObject(brush_w);
		dc->Ellipse(o.x - 15, o.y - 15, o.x + 15, o.y + 15);
	}
	else if (color == 1)//黑棋
	{
		CBrush brush_b(RGB(0, 0, 0));
		const CPoint o(50 * nx + 50, 50 * ny + 50);//圆心
		dc->SelectObject(brush_b);
		dc->Ellipse(o.x - 15, o.y - 15, o.x + 15, o.y + 15);
	}
}

void CGoBangDlg::EndGame()
{
	CleanChessBoard();
	IsPlaying = false;
	GetDlgItem(IDC_START)->SetWindowTextW(L"开始游戏");
	GetDlgItem(IDC_ENDGAME)->EnableWindow(FALSE);
}

void CGoBangDlg::CleanChessBoard()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			ChessBoard[i][j] = -1;
		}
	}
	RECT rect;
	GetClientRect(&rect);
	InvalidateRect(&rect);
}


BOOL CGoBangDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if (!IsPlaying || point.x < 40 || point.x>760 || point.y < 40 || point.y>760)
		return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	if (NowColor == 1)//黑棋
		SetCursor(LoadCursorW(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR1)));
	else
		SetCursor(LoadCursorW(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDC_CURSOR2)));
	return TRUE;
}


void CGoBangDlg::OnClose()
{
	if (!IsPlaying || MessageBoxW(L"正在游戏中，确定要退出吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDYES)
		CDialogEx::OnClose();
}


void CGoBangDlg::OnBnClickedEndgame()
{
	if (MessageBoxW(L"确定要结束本局吗？", L"双人五子棋", MB_YESNO | MB_ICONQUESTION) == IDYES)
		EndGame();
}
