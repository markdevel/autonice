
// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Windows ヘッダーから使用されていない部分を除外します。
#endif

#include "targetver.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <winsvc.h>
#include <evntprov.h>
#include <Evntrace.h>
