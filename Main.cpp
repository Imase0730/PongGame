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
	// ----- 定数宣言 ----- //

	// 画面のサイズ
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	// パドルの大きさ
	const int PADDLE_WIDTH = 32;
	const int PADDLE_HEIGHT = 64;

	// パドルの移動速度
	const int PADDLE_SPEED = 8;

	// パドルの初期位置
	const int PADDLE_X = 100;
	const int PADDLE_Y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;

	// ----- 変数宣言 ----- //
	int paddleX;
	int paddleY;
	int key;

	// ----- 変数の初期化 ----- //
	paddleX = PADDLE_X;
	paddleY = PADDLE_Y;
	key = 0;

	// 画面モードのセット
	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

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

		// キー入力の取得
		key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// 上キーが押された
		if (key & PAD_INPUT_UP)
		{
			paddleY -= PADDLE_SPEED;
		}

		// 下キーが押された
		if (key & PAD_INPUT_DOWN)
		{
			paddleY += PADDLE_SPEED;
		}
		
		// 画面外へパドルが移動しないように画面外なら位置を補正する
		if (paddleY < 0)
		{
			paddleY = 0;
		}
		if (paddleY > SCREEN_HEIGHT - PADDLE_HEIGHT)
		{
			paddleY = SCREEN_HEIGHT - PADDLE_HEIGHT;
		}

		// 画面を初期化する
		ClearDrawScreen();

		// ゲームの描画処理

		// 四角形の描画
		DrawBox(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT, GetColor(255, 255, 255), TRUE);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
