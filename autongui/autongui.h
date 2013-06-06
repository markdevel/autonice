
// autongui.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CAutoNiceGuiApp:
// このクラスの実装については、autongui.cpp を参照してください。
//

class CAutoNiceGuiApp : public CWinApp
{
public:
	CAutoNiceGuiApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CAutoNiceGuiApp theApp;