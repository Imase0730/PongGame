// キー入力基本
#include "DxLib.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	// 画面モードのセット
	SetGraphMode(1280, 720, 32);

	// タイトルを変更
	SetMainWindowText(L"Pong Game");

	// ウインドウモードに変更
	ChangeWindowMode(TRUE);

	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ゲームの更新処理

		// 画面を初期化する
		ClearDrawScreen();

		// ゲームの描画処理

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
