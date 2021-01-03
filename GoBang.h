
// GoBang.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号
#define SIZE 15

// CGoBangApp:
// 有关此类的实现，请参阅 GoBang.cpp
//

class CGoBangApp : public CWinApp
{
public:
	CGoBangApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CGoBangApp theApp;

int GetChessBoardColor(int nx, int ny);
int GetChessCount(int nx, int ny, CPoint*, CPoint*);
int GetWinner();//获取赢家，-1无,0白,1黑
int GetResult(int nx, int ny);
extern int ChessBoard[SIZE][SIZE];//棋盘，-1为空，0为白，1为黑