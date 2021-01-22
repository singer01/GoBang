
// GoBangDlg.h: 头文件
//

#pragma once

#define SIZE 15

// CGoBangDlg 对话框
class CGoBangDlg : public CDialogEx
{
// 构造
public:
	CGoBangDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOBANG_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	bool IsPlaying;
	bool NowColor;
	int ChessBoard[SIZE][SIZE];//棋盘，-1为空，0为白，1为黑
	int index;
	CPoint order[SIZE * SIZE];
	int GetChessBoardColor(int ,int);
	void SetChessBoardColor(int ,int,int);
	void EndGame();
	void CleanChessBoard();
	void OpenFile(CString filename);
	int GetChessCount(int,int);
	int GetWinner();//获取赢家，-1无,0白,1黑

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	afx_msg void OnBnClickedEndgame();
	afx_msg void OnBnClickedRepentance();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedOpen();
};
