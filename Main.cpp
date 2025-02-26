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

	// フォントのサイズ
	const int FONT_SIZE = 50;

	// パドルの大きさ
	const int PADDLE_WIDTH = 32;
	const int PADDLE_HEIGHT = 64;

	// パドルの移動速度
	const int PADDLE_SPEED = 8;

	// パドルの初期位置
	const int PADDLE_X = 100;
	const int PADDLE_Y = SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2;

	// ボールの大きさ
	const int BALL_SIZE = 32;

	// ----- 変数宣言 ----- //
	int key;
	int paddleX;
	int paddleY;
	int ballX;
	int ballY;
	int ballVelocityX;
	int ballVelocityY;
	int score;

	// ----- 変数の初期化 ----- //
	key = 0;
	paddleX = PADDLE_X;
	paddleY = PADDLE_Y;
	ballX = 0;
	ballY = 0;
	ballVelocityX = 5;
	ballVelocityY = 5;
	score = 0;

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

	// フォントのサイズの設定
	SetFontSize(FONT_SIZE);

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

		// ボールの移動
		ballX += ballVelocityX;
		ballY += ballVelocityY;
		
		// 右側の壁に当たった時
		if (ballX >= SCREEN_WIDTH - BALL_SIZE)
		{
			// 跳ね返る
			ballVelocityX = -ballVelocityX;
		}

		// 左側の壁に当たった時
		if (ballX <= 0)
		{
			// 跳ね返る
			ballVelocityX = -ballVelocityX;
		}

		// 上側の壁に当たった時
		if (ballY <= 0)
		{
			// 跳ね返る
			ballVelocityY = -ballVelocityY;
		}

		// 下側の壁に当たった時
		if (ballY >= SCREEN_HEIGHT - BALL_SIZE)
		{
			// 跳ね返る
			ballVelocityY = -ballVelocityY;
		}

		// パドルとボールの衝突判定
		if ( (ballVelocityX < 0)	// ボールが左方向へ移動している
		  && (paddleX < ballX + BALL_SIZE && ballX < paddleX + BALL_SIZE)
		  && (paddleY < ballY + BALL_SIZE && ballY < paddleY + BALL_SIZE)
		   )
		{
			ballVelocityX = -ballVelocityX;
			// スコアを加算
			score++;
		}

		// 画面を初期化する
		ClearDrawScreen();

		// ゲームの描画処理

		// パドルの描画
		DrawBox(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT, GetColor(255, 255, 255), TRUE);

		// ボールの描画
		DrawBox(ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE, GetColor(255, 255, 255), TRUE);

		// スコアの表示
		DrawFormatString(0, 0, GetColor(255, 255, 255), L"SCORE:%d", score);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
