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

	// シーンの列挙子
	enum class Scene
	{
		TITLE,
		GAME,
		GAMEOVER,
	};

	// タイトルの文字列
	const wchar_t TITLE[] = L"Pong Game";

	// ゲームオーバーの文字列
	const wchar_t GAMEOVER[] = L"Game Over";

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

	// ボールの初期位置
	const int BALL_X = SCREEN_WIDTH - BALL_SIZE;
	const int BALL_Y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;

	// ボールの初期速度
	const int BALL_SPEED_X = -5;
	const int BALL_SPEED_Y = 5;

	// ----- 変数宣言 ----- //
	int key;
	int oldKey;
	int paddleX;
	int paddleY;
	int ballX;
	int ballY;
	int ballVelocityX;
	int ballVelocityY;
	int score;
	Scene scene;

	// ----- 変数の初期化 ----- //
	key = 0;
	oldKey = 0;
	paddleX = PADDLE_X;
	paddleY = PADDLE_Y;
	ballX = BALL_X;
	ballY = BALL_Y;
	ballVelocityX = BALL_SPEED_X;
	ballVelocityY = BALL_SPEED_Y;
	score = 0;
	scene = Scene::TITLE;

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
		// ----- ゲームの更新処理 ----- //

		// キー入力の取得
		oldKey = key;
		key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		// タイトルなら
		if (scene == Scene::TITLE)
		{
			// スペースキーでゲーム開始
			if ((~oldKey & key) & PAD_INPUT_10)
			{
				// ゲーム中へ
				scene = Scene::GAME;
				// ゲームで使用する変数の初期化
				paddleX = PADDLE_X;
				paddleY = PADDLE_Y;
				ballX = BALL_X;
				ballY = BALL_Y;
				ballVelocityX = BALL_SPEED_X;
				ballVelocityY = BALL_SPEED_Y;
				score = 0;
			}
		}

		// ゲーム中なら
		if (scene == Scene::GAME)
		{
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
				// ゲームオーバーへ
				scene = Scene::GAMEOVER;
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
			if ((ballVelocityX < 0)	// ボールが左方向へ移動している
				&& (paddleX < ballX + BALL_SIZE && ballX < paddleX + BALL_SIZE)
				&& (paddleY < ballY + BALL_SIZE && ballY < paddleY + BALL_SIZE)
				)
			{
				ballVelocityX = -ballVelocityX;
				// スコアを加算
				score++;
			}
		}

		// ゲームオーバーなら
		if (scene == Scene::GAMEOVER)
		{
			// スペースキーでタイトルへ
			if ((~oldKey & key) & PAD_INPUT_10)
			{
				// タイトルへ
				scene = Scene::TITLE;
			}
		}

		// 画面を初期化する
		ClearDrawScreen();

		// ----- ゲームの描画処理 ----- //

		// タイトルなら
		if (scene == Scene::TITLE)
		{
			// ゲームタイトルの表示
			DrawFormatString(SCREEN_WIDTH / 2 - GetDrawStringWidth(TITLE, wcslen(TITLE)) / 2
				, SCREEN_HEIGHT / 2 - FONT_SIZE / 2
				, GetColor(255, 255, 255), TITLE, score);
		}

		// ゲーム中なら
		if (scene == Scene::GAME)
		{
			// パドルの描画
			DrawBox(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT, GetColor(255, 255, 255), TRUE);

			// ボールの描画
			DrawBox(ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE, GetColor(255, 255, 255), TRUE);

			// スコアの表示
			DrawFormatString(0, 0, GetColor(255, 255, 255), L"SCORE:%d", score);
		}

		// ゲームオーバーなら
		if (scene == Scene::GAMEOVER)
		{
			// パドルの描画
			DrawBox(paddleX, paddleY, paddleX + PADDLE_WIDTH, paddleY + PADDLE_HEIGHT, GetColor(255, 255, 255), TRUE);

			// ボールの描画
			DrawBox(ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE, GetColor(255, 255, 255), TRUE);

			// スコアの表示
			DrawFormatString(0, 0, GetColor(255, 255, 255), L"SCORE:%d", score);

			// GAMEOVERの表示
			DrawFormatString(SCREEN_WIDTH / 2 - GetDrawStringWidth(GAMEOVER, wcslen(GAMEOVER)) / 2
				, SCREEN_HEIGHT / 2 - FONT_SIZE / 2
				, GetColor(255, 255, 255), GAMEOVER, score);
		}

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
