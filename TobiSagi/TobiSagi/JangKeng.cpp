#include "StdAfx.h"
#include "stdio.h"
#include "JangKeng.h"


using namespace TobiSagi;

// ゲーム初期化
void JangKeng::InitializeStart(void)
{
	this->s_watch = gcnew Diagnostics::Stopwatch();
	this->rnd = gcnew Random();
	this->status = STS_TITLE;
	this->game_cnt = 0;
	this->truemode = false;

	// ボタンのみ表示しておきましょか。
	this->panel1->Visible = false;
	this->panel_select->Visible = false;
	this->panel_pre->Visible = false;
	this->pictureBox3->Visible = false;

	// イメージ画像をロードする
	bmp_guu1 = gcnew Bitmap( L"01_goo.jpg" );
	bmp_choki1 = gcnew Bitmap( L"02_choki.jpg" );
	bmp_paa1 = gcnew Bitmap( L"03_paa.jpg" );
	bmp_guu2 = gcnew Bitmap( L"01_goo.jpg" );
	bmp_guu2->RotateFlip( RotateFlipType::RotateNoneFlipX );
	bmp_choki2 = gcnew Bitmap( L"02_choki.jpg" );
	bmp_choki2->RotateFlip( RotateFlipType::RotateNoneFlipX );
	bmp_paa2 = gcnew Bitmap( L"03_paa.jpg" );
	bmp_paa2->RotateFlip( RotateFlipType::RotateNoneFlipX );
	bmp_win = gcnew Bitmap( L"win01.jpg" );
	bmp_lose = gcnew Bitmap( L"lose01.jpg" );
	bmp_reach_player = gcnew Bitmap( L"reach_player.jpg" );
	bmp_reach_computer = gcnew Bitmap( L"reach_computer.jpg" );
	bmp_true_ending = gcnew Bitmap ( L"true_ending.jpg" );
	bmp_gameover = gcnew Bitmap( L"gameover.jpg" );
}

// ゲームスタート時の初期化
void JangKeng::InitializeGame(void)
{
	cnt_game = 0;
	cnt_win_player = 0;
	cnt_win_com = 0;
	mode = MODE_NOMAL;	// 最初はノーマルモード
	aiko_level = 0;
	game_cnt ++;

	// ボタンやコントローラーの表示を変更する
	this->panel1->Visible = true;
	this->button1->Visible = false;
	this->timer1->Enabled = false;
	this->panel_select->Visible = false;	// 入力ボタンパネルを隠す。
	this->panel_pre->Visible = false;
	this->pictureBox3->Visible = false;
	this->label_reach_player->Visible = false;
	this->label_reach_computer->Visible = false;
	this->label_hint->Visible = false;
}

// ステータス判定＆遷移処理
void JangKeng::CheckStatus(void)
{
	switch( this->status )
	{
	case STS_TITLE:
		InitializeGame();
		this->status = STS_TURN_OPENING;
		StsTurnOpening();
		break;

	case STS_TURN_OPENING:
		this->status = STS_GAME_PRE_ACTION;
		StsGamePreAction();
		break;

	case STS_GAME_PRE_ACTION:
		this->status = STS_GAME_INPUT_WAIT;
		StsGameInputWait();
		break;

	case STS_GAME_INPUT_WAIT:
		if( TE_NONE == this->choice_player )
		{	// 演出
			StsGameInputWaitAnime();
		} else {
			// 次のステータスへ
			this->status = STS_GAME_JUDGE;
			StsGameJudge();
		}
		break;

	case STS_GAME_JUDGE:
		// しょうがないので「腕が傾いた状態」をここで元に戻す
		this->pictureBox1->Image->RotateFlip( RotateFlipType::Rotate270FlipNone );
		this->pictureBox2->Image->RotateFlip( RotateFlipType::Rotate90FlipNone );
		this->panel_pre->Visible = false;	// 掛け声パネルを隠す。

		// じゃんけん判定を行う。
		switch( CheckJangKengWinner( this->choice_player, this->choice_enemy ) )
		{
		case PL_PLAYER:
			this->status = STS_ONE_GAME_WIN;
			StsOneGameWin();
			break;

		case PL_COMPUTER:
			this->status = STS_ONE_GAME_LOSE;
			StsOneGameLose();
			break;

		default:
			// あいこの場合
			this->cnt_aiko ++;
			this->status = STS_GAME_PRE_ACTION;
			StsGamePreAction();
			break;
		}
		break;

	case STS_ONE_GAME_WIN:
		// ここで表示を消す
		this->pictureBox3->Visible = false;
		// リーチ判定
		if( this->cnt_win_player == GAME_REACH_COUNT )
		{	// リーチ画面へ
			this->status = STS_REACH_PLAYER;
			StsReachPlayer();
			// エンディング判定
		} else if( this->cnt_win_player == GAME_WIN_COUNT )
		{	// エンディングへ
			this->status = STS_GAME_TRUE_ENDING;
			StsGameTrueEnding();
		} else {
			this->status = STS_TURN_OPENING;
			StsTurnOpening();
		}
		break;

	case STS_REACH_PLAYER:
		this->pictureBox3->Visible = false;		// ここで表示を消す
		this->status = STS_TURN_OPENING;
		StsTurnOpening();
		break;

	case STS_ONE_GAME_LOSE:
		this->pictureBox3->Visible = false;		// ここで表示を消す
		// リーチ判定
		if( this->cnt_win_com == GAME_REACH_COUNT )
		{
			this->status = STS_REACH_COMPUTER;
			StsReachComputer();
		// ゲームオーバー判定
		} else if( this->cnt_win_com == GAME_WIN_COUNT )
		{	// コンピューターの勝利！
			this->status = STS_GAME_OVER;
			StsGameOver();
		} else {	// まだ。
			this->status = STS_TURN_OPENING;
			StsTurnOpening();
		}
		break;

	case STS_REACH_COMPUTER:
		this->pictureBox3->Visible = false;		// ここで表示を消す
		this->status = STS_TURN_OPENING;
		StsTurnOpening();
		break;

	default:
		break;
	}
}

// 「ｎ回戦目です」
void JangKeng::StsTurnOpening(void)
{
	this->cnt_game ++;		// ゲーム数＋１
	this->cnt_aiko = 0;		// あいこカウント初期化

	// 表示文字列
	System::String ^str;
	str = "第 " + this->cnt_game + " 回戦、スタート！";
	this->label1->Text = str;	// sprintf_s( "hoge" );
	str = "勝ち= " + this->cnt_win_player;
	this->label_player_win->Text = str;
	str = "勝ち= " + this->cnt_win_com;
	this->label_com_win->Text = str;

	// リーチ表示判定
	if( this->cnt_win_player == GAME_REACH_COUNT )
	{
		this->label_reach_player->Visible = true;
	}
	if( this->cnt_win_com == GAME_REACH_COUNT )
	{
		this->label_reach_computer->Visible = true;
	}

	// 表示調整
	this->panel1->Visible = true;	// 「ｎ回戦」パネル表示

	// コンピューターモード更新
	RenewComMode();

	// タイマー起動
	this->timer1->Interval = 1500;
	this->timer1->Enabled = true;
}

// 「詐欺・指向ルーチン」コンピューターモード更新
void JangKeng::RenewComMode(void)
{
	// プレイヤーが勝ちかけなら、詐欺モードにする
	if( this->cnt_win_player >= 5 )
	{
		this->mode = MODE_SAGI;	// 詐欺モード

		if( this->cnt_win_com >= 5 )
		{	// 最終決戦！（５ＶＳ５）
			this->aiko_level = this->rnd->Next( 5 ) + 5;	// 5～9回のあいこ。
		} else {
			this->aiko_level = this->rnd->Next( 3 );		// 0～2回のあいこ。
		}
		return;
	}

	// コンピューターが勝ちかけなら、負けモードにする
	if( this->cnt_win_com >= 5 )
	{
		this->mode = MODE_MAKE;		// 負けモード
		this->aiko_level = this->rnd->Next( 3 ) + 2;	// 2～4回のあいこ。
		return;
	}

	// プレイヤー最初の２戦は、いきなり勝たせてやる。
	if( this->cnt_win_player <= 1 )
	{
		this->mode = MODE_MAKE;		// 負けモード
		this->aiko_level = this->rnd->Next( 2 ) + this->cnt_win_com;	// 1回目は0-1回、2回目は1-2回のあいこ。
		return;
	}

	// プレイヤーが３勝している状態で敵が０勝だったら、１回くらい敵に勝たせる。
	if( this->cnt_win_player == 3 && this->cnt_win_com == 0 )
	{
		this->mode = MODE_SAGI;	// 詐欺モード
		this->aiko_level = this->rnd->Next( 2 );	// 0-1回のあいこ。
		return;
	}

	this->mode = MODE_NOMAL;		// ノーマルモード
	this->aiko_level = 0;
}

// 「じゃーん・けーん」
void JangKeng::StsGamePreAction(void)
{
	// 表示調整
	this->panel1->Visible = false;	// 「ｎ回戦」パネルを隠す。
	if( this->cnt_aiko == 0 )
	{
		this->label2->Text = L"じゃ～んけ～ん！";
		this->timer1->Interval = 1200;
	} else {
		this->label2->Text = L"あ～いこ～で！";
		this->timer1->Interval = 800;
	}
	this->panel_pre->Visible = true; 

	// タイマー起動
	this->timer1->Enabled = true;
}

// プレイヤー入力まち
void JangKeng::StsGameInputWait(void)
{
	// 表示調整
//	this->panel_pre->Visible = false;	// 「じゃーんけーん」パネルを隠す。
	this->panel_select->Visible = true;	// 入力ボタンパネルを表示。

	// パラメーター設定
	this->choice_player = TE_NONE;

	// タイマー起動
	this->timer1->Interval = 100;	// 高速回転
	this->timer1->Enabled = true;

	// アニメ（？）表示
	StsGameInputWaitAnime();
}

// ランダムでグー・チョキ・パーを表示
void JangKeng::StsGameInputWaitAnime(void)
{
	// 乱数表示しつつ、手の角度を「上」に設定する
	this->pictureBox1->Image = this->SetJangKengHand( PL_PLAYER, this->rnd->Next(3) );
	this->pictureBox2->Image = this->SetJangKengHand( PL_COMPUTER, this->rnd->Next(3) );
}

// じゃんけんイメージ画像を返す
Bitmap ^JangKeng::SetJangKengHand( int player, int num )
{
	if( TE_GUU == num )
	{
		if( PL_PLAYER == player ) return bmp_guu1;
		return bmp_guu2;
	}
	if( TE_CHOKI == num )
	{
		if( PL_PLAYER == player ) return bmp_choki1;
		return bmp_choki2;
	}
	if( PL_PLAYER == player ) return bmp_paa1;
	return bmp_paa2;
}

// ジャッジ処理
void JangKeng::StsGameJudge(void)
{
	// 表示調整
	this->panel_select->Visible = false;	// 入力ボタンパネルを隠す。

	// 掛け声調整
	switch( this->rnd->Next(10) )
	{
	case 0:		this->label2->Text = L"うりゃっ！";		break;
	case 1:		this->label2->Text = L"どりゃっ！";		break;
	case 2:		this->label2->Text = L"そいやっ！";		break;
	case 3:		this->label2->Text = L"・・・？！";		break;
	case 4:		this->label2->Text = L"ごるぁ！！";		break;
	case 5:		this->label2->Text = L"なん…だと？！";		break;
	case 6:		this->label2->Text = L"ぽんっ！！";		break;
	case 7:		this->label2->Text = L"どかん！！";		break;
	case 8:		this->label2->Text = L"ほいっ！！";		break;
	case 9:		this->label2->Text = L"ほっす！！";		break;
	}

	// コンピューターの手を計算する（詐欺）
	this->choice_enemy = GetComJangKeng();

	// プレイヤーの手とコンピューターの手を表示
	this->pictureBox1->Image = this->SetJangKengHand( PL_PLAYER, this->choice_player );
	this->pictureBox1->Image->RotateFlip( RotateFlipType::Rotate90FlipNone );

	this->pictureBox2->Image = this->SetJangKengHand( PL_COMPUTER, this->choice_enemy );
	this->pictureBox2->Image->RotateFlip( RotateFlipType::Rotate270FlipNone );

	// タイマー起動
	this->timer1->Interval = 1000;
	this->timer1->Enabled = true;
}

// コンピューターの手を計算する（詐欺）
int JangKeng::GetComJangKeng(void)
{
	int rt;

	switch( mode )
	{
	case MODE_NOMAL:
		rt = this->rnd->Next(3);	// 乱数でジャンケン
		break;

	case MODE_SAGI:
		if( this->cnt_aiko < this->aiko_level )
		{	// あいこレベル回数だけあいこにする
			rt = this->choice_player;					// あいこにする
		} else {
			// trueモード判定
			if( this->cnt_win_player == 5 && this->cnt_win_com == 5 && this->truemode )
			{
				rt = this->GetLoseChoice( this->choice_player );	// エンディング用処理
			} else {
				rt = this->GetWinChoice( this->choice_player );	// コンピューターの勝ちにする
			}
		}
		break;

	case MODE_MAKE:
	default:
		if( this->cnt_aiko < this->aiko_level )
		{	// あいこレベル回数だけあいこにする
			rt = this->choice_player;					// あいこにする
		} else {
			rt = this->GetLoseChoice( this->choice_player );		// コンピューターの負けにする
		}
		break;
	}
	return rt;
}

// 入力に対し「勝つ」手を返す
int JangKeng::GetWinChoice( int te )
{
	int rt;

	switch( te )
	{
	case TE_GUU:				rt = TE_PAA;	break;		// パーなら勝てる。
	case TE_CHOKI:				rt = TE_GUU;	break;		// グーなら勝てる。
	case TE_PAA:	default:	rt = TE_CHOKI;	break;		// チョキなら勝てる。
	}
	return rt;
}

// 入力に対し「負ける」手を返す
int JangKeng::GetLoseChoice( int te )
{
	int rt;

	switch( te )
	{
	case TE_GUU:				rt = TE_CHOKI;	break;		// チョキなら負ける。
	case TE_CHOKI:				rt = TE_PAA;	break;		// パーなら負ける。
	case TE_PAA:	default:	rt = TE_GUU;	break;		// グーなら負ける。
	}
	return rt;
}

// ジャンケンの勝者がだれかを返す関数
int JangKeng::CheckJangKengWinner( int pl, int com )
{
	if( pl == TE_GUU )
	{
		if( com == TE_GUU ) return PL_AIKO;			// あいこ
		if( com == TE_CHOKI ) return PL_PLAYER;		// プレイヤーの勝ち
		return PL_COMPUTER;							// コンピューターの勝ち
	} else if( pl == TE_CHOKI )
	{
		if( com == TE_GUU ) return PL_COMPUTER;		// コンピューターの勝ち
		if( com == TE_CHOKI ) return PL_AIKO;		// あいこ
		return PL_PLAYER;							// プレイヤーの勝ち
	}
	if( com == TE_GUU ) return PL_PLAYER;			// プレイヤーの勝ち
	if( com == TE_CHOKI ) return PL_COMPUTER;		// コンピューターの勝ち
	return PL_AIKO;									// あいこ
}

// プレイヤーの勝ち！
void JangKeng::StsOneGameWin(void)
{
	this->cnt_win_player ++;	// プレイヤー勝ち数＋１

	// 表示調整
	this->pictureBox3->BringToFront();			// コントロールを最前面へ。
	this->pictureBox1->BringToFront();
	this->pictureBox2->BringToFront();
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_win;
	this->pictureBox3->Visible = true;	

	// タイマー起動
	this->timer1->Interval = 2000;
	this->timer1->Enabled = true;
}

// プレイヤー・リーチ！
void JangKeng::StsReachPlayer(void)
{
	// 表示調整
	this->pictureBox3->BringToFront();			// コントロールを最前面へ。
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_reach_player;
	this->pictureBox3->Visible = true;	

	// タイマー起動
	this->timer1->Interval = 3500;
	this->timer1->Enabled = true;
}

// コンピューターの勝ち！
void JangKeng::StsOneGameLose(void)
{
	this->cnt_win_com ++;	// コンピューター勝ち数＋１

	// 表示調整
	this->pictureBox3->BringToFront();			// コントロールを最前面へ。
	this->pictureBox3->Location = System::Drawing::Point(124, 100);
	this->pictureBox3->Size = System::Drawing::Size(370, 200);
	this->pictureBox3->Image = this->bmp_lose;
	this->pictureBox3->Visible = true;	

	// タイマー起動
	this->timer1->Interval = 2000;
	this->timer1->Enabled = true;
}

// コンピューター・リーチ！
void JangKeng::StsReachComputer(void)
{
	// 表示調整
	this->pictureBox3->BringToFront();			// コントロールを最前面へ。
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_reach_computer;
	this->pictureBox3->Visible = true;	

	// タイマー起動
	this->timer1->Interval = 3500;
	this->timer1->Enabled = true;
}

// ゲームオーバー
void JangKeng::StsGameOver(void)
{
	// 表示調整
	this->pictureBox3->BringToFront();			// コントロールを最前面へ。
	this->pictureBox3->Location = System::Drawing::Point(140, 95);
	this->pictureBox3->Size = System::Drawing::Size(320, 210);
	this->pictureBox3->Image = this->bmp_gameover;
	this->pictureBox3->Visible = true;	

	// ラストメッセージ表示
	switch( this->game_cnt )
	{
	case 1:	this->label_hint->Text = L"m9(^Д^)プギャー！！！ｗｗｗ";			break;
	case 2: this->label_hint->Text = L"ワロスｗｗｗテラワロスｗｗｗ";			break;
	case 3: this->label_hint->Text = L"アンタもしつこいねぇ。無理だってばよｗｗ";	break;
	case 4: this->label_hint->Text = L"…。ヒントは「３対３でやり直し」だよ。";	break;
	default: this->label_hint->Text = L"もう諦めなってｗｗｗ無駄無駄無駄無駄ぁぁ！！！"; break;
	}
	this->label_hint->Visible = true;
}

// trueエンディング
void JangKeng::StsGameTrueEnding(void)
{
	// 表示調整
	this->pictureBox3->BringToFront();			// コントロールを最前面へ。
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_true_ending;
	this->pictureBox3->Visible = true;	
	this->label_hint->BringToFront();
	// ラストメッセージ
	this->label_hint->Text = L"おめでとう！よくここまでたどり着いたね！よっぽど暇なんだね！ゲームクリア！！";
	this->label_hint->Visible = true;
}

// スタートボタン押下
System::Void JangKeng::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	CheckStatus();
}

// グーボタン
System::Void JangKeng::button_guu_Click(System::Object^  sender, System::EventArgs^  e)
{
	choice_player = TE_GUU;
	CheckStatus();
}

// チョキボタン
System::Void JangKeng::button_choki_Click(System::Object^  sender, System::EventArgs^  e)
{
	choice_player = TE_CHOKI;
	CheckStatus();
}

// パーボタン
System::Void JangKeng::button_paa_Click(System::Object^  sender, System::EventArgs^  e)
{
	choice_player = TE_PAA;
	CheckStatus();
}

// タイマー満了
System::Void JangKeng::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	CheckStatus();
}

// リプレイボタン
System::Void JangKeng::button_replay_Click(System::Object^  sender, System::EventArgs^  e)
{	// 強制的にゲームリセット。最初から始める。
	if( this->cnt_win_player == 3 && this->cnt_win_com == 3 ) 
	{
		this->truemode = true;	// フラグ判定
	}
	this->status = STS_TITLE;
	CheckStatus();
}
