『AutoNice』プロセス優先度の自動設定ツール
==========================================

## 特徴

* 一定時間ごとに指定したファイル名のプロセスの優先度を変更します。
* サービスとしてバックグラウンドで動作します。
* フルC/C++実装のため、軽快かつメモリの使用量が少なめです。
* 32ビット・64ビットそれぞれネイティブに対応しています。

## 動作環境

* Windows 7 SP1 32ビット, 64ビット
* Windows 8.1 Update 32ビット, 64ビット
* Windows 10 Version 1803 以降 32ビット, 64ビット

(テスト対象: Windows 10 Version 1809 64ビット)

## インストール

1. Microsoft Visual C++ 2015-2019 Redistributable をインストールする。
   * 32ビット  
     https://aka.ms/vs/16/release/VC_redist.x86.exe
   * 64ビット  
     https://aka.ms/vs/16/release/VC_redist.x64.exe
2. インストーラー（32ビット・64ビット共通）をダウンロードし、実行する。
   * 最新版  
     https://github.com/markdevel/autonice/releases/download/v1.2.0/setup-autonice-1.2.0.exe  
     MD5: 15b9211479345c037b3fcbc020e32998
   * 過去のバージョン  
     https://github.com/markdevel/autonice/releases/download/v1.1.0/setup-autonice-1.1.0.exe  
     MD5: 660edfd096153396e67786c63239405d

## 使い方

1. スタートメニューから AutoNice を起動する。
2. 『追加』ボタンから優先度を変更したいプロセスを設定する。
3. 『OK』ボタンを押して、メインウィンドウを閉じる。
4. 5秒程度で設定が反映されるので、タスクマネージャー等で効果を確認する。

## ライセンス

* GPLv3

## 更新履歴

* 1.2 (2019-04-10)
  - Windows Vista以下をサポート対象外とした
  - 安定性向上のためのリファクタリング
  - 細かなバグ修正
* 1.1 (2015-01-18)
  - インストーラーの統合
  - 細かなバグ修正
* 1.0 (2013-06-09)
  - 初版
