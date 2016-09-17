#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace TobiSagi {

#define GAME_REACH_COUNT	5		// リーチ数は「５」・・・。４にすればよかった。
#define GAME_WIN_COUNT	6			// 勝利数は「６」・・・。５にすればよかった。

	typedef enum game_status {
		STS_TITLE	= 0,			// タイトル画面
		STS_TURN_OPENING = 1,		// ターン・オープニング
		STS_GAME_PRE_ACTION = 2,	// 「じゃーん・けーん」
		STS_GAME_INPUT_WAIT = 3,	// プレイヤー入力まち
		STS_GAME_JUDGE = 4,			// 「ぽん！」ジャッジ
		STS_ONE_GAME_WIN = 5,		// 勝ち！
		STS_ONE_GAME_LOSE = 6,		// 負け！
		STS_ONE_GAME_AIKO = 7,		// あいこ
		STS_REACH_PLAYER = 8,		// プレイヤーがリーチ
		STS_REACH_COMPUTER = 9,		// コンピューターがリーチ
		STS_GAME_TRUE_ENDING = 10,	// trueエンディング
		STS_GAME_OVER = 11			// ゲーム終了
	}gs;

	typedef enum game_te {
		TE_GUU = 0,		// グー
		TE_CHOKI = 1,	// チョキ
		TE_PAA = 2,		// パー
		TE_NONE = 3		// まだ選んでない
	}te;

	typedef enum player {	// プレイヤー・コンピューターの識別にも使うし、あいこ時フラグにも使う。
		PL_PLAYER = 0,		// プレイヤー
		PL_COMPUTER = 1,	// コンピューター
		PL_AIKO = 2			// あいこ
	}pl;

	typedef enum commode {
		MODE_NOMAL = 0,		// ノーマルモード
		MODE_SAGI = 1,		// 詐欺モード
		MODE_MAKE = 2		// 負けるモード
	}md;

	/// <summary>
	/// JangKeng の概要
	///
	/// 警告: このクラスの名前を変更する場合、このクラスが依存するすべての .resx ファイルに関連付けられた
	///          マネージ リソース コンパイラ ツールに対して 'Resource File Name' プロパティを
	///          変更する必要があります。この変更を行わないと、
	///          デザイナと、このフォームに関連付けられたローカライズ済みリソースとが、
	///          正しく相互に利用できなくなります。
	/// </summary>
	public ref class JangKeng : public System::Windows::Forms::Form
	{
	// 変数群
	private:
		Diagnostics::Stopwatch		^s_watch;	// ストップウォッチ
		int status;								// ゲーム・ステータス
		int cnt_game;		// ゲーム・カウント
		int cnt_win_player;	// プレイヤー勝ち数
		int cnt_win_com;	// コンピューター勝ち数
		int cnt_aiko;		// アイコ回数カウンタ
		int choice_player;	// プレイヤーの選択
		int choice_enemy;	// 敵の選択
		int game_cnt;		// ゲームカウント
		bool truemode;		// trueモードフラグ
		Bitmap ^bmp_guu1;	// グー
		Bitmap ^bmp_choki1;	// チョキ
		Bitmap ^bmp_paa1;	// パー
		Bitmap ^bmp_guu2;	// グー
		Bitmap ^bmp_choki2;	// チョキ
		Bitmap ^bmp_paa2;	// パー
		Bitmap ^bmp_win;	// 勝ち画面
		Bitmap ^bmp_lose;	// 負け画面
		Bitmap ^bmp_reach_player;	// リーチ・プレイヤー
		Bitmap ^bmp_reach_computer;	// リーチ・コンピューター
		Bitmap ^bmp_true_ending;	// trueエンディング画面
		Bitmap ^bmp_gameover;	// ゲームオーバー画面
		Random ^rnd;		// 乱数発生
		int mode;			// コンピューターモード
		int aiko_level;		// あいこレベル

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::Panel^  panel_select;
	private: System::Windows::Forms::Button^  button_paa;
	private: System::Windows::Forms::Button^  button_choki;
	private: System::Windows::Forms::Button^  button_guu;
	private: System::Windows::Forms::Panel^  panel_pre;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::Label^  label_player_win;
	private: System::Windows::Forms::Label^  label_com_win;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button1;
	private: System::ComponentModel::IContainer^  components;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  button_replay;
	private: System::Windows::Forms::Label^  label_reach_player;
	private: System::Windows::Forms::Label^  label_reach_computer;
	private: System::Windows::Forms::Label^  label_hint;
	private: System::Windows::Forms::Timer^  timer1;

	public:
		JangKeng(void)
		{
			InitializeComponent();
			InitializeStart();			// ゲーム初期化
			//
			//TODO: ここにコンストラクタ コードを追加します
			//
		}

		void InitializeStart(void);
		void InitializeGame(void);
		void CheckStatus(void);
		void StsTurnOpening(void);
		void RenewComMode(void);
		void StsGamePreAction(void);
		void StsGameInputWait(void);
		void StsGameJudge(void);
		int GetComJangKeng(void);
		void StsGameInputWaitAnime(void);	// ほげる
		Bitmap ^SetJangKengHand( int pl, int num );
		int GetWinChoice( int te );
		int GetLoseChoice( int te );
		int CheckJangKengWinner( int pl, int com );
		void StsOneGameWin(void);
		void StsReachPlayer(void);
		void StsOneGameLose(void);
		void StsReachComputer(void);
		void StsGameTrueEnding(void);
		void StsGameOver(void);

	protected:
		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		~JangKeng()
		{
			if (components)
			{
				delete components;
			}
		}
	protected: 

	private:
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->panel_select = (gcnew System::Windows::Forms::Panel());
			this->button_paa = (gcnew System::Windows::Forms::Button());
			this->button_choki = (gcnew System::Windows::Forms::Button());
			this->button_guu = (gcnew System::Windows::Forms::Button());
			this->panel_pre = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->label_player_win = (gcnew System::Windows::Forms::Label());
			this->label_com_win = (gcnew System::Windows::Forms::Label());
			this->button_replay = (gcnew System::Windows::Forms::Button());
			this->label_reach_player = (gcnew System::Windows::Forms::Label());
			this->label_reach_computer = (gcnew System::Windows::Forms::Label());
			this->label_hint = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->panel_select->SuspendLayout();
			this->panel_pre->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(182, 317);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(232, 52);
			this->button1->TabIndex = 0;
			this->button1->Text = L"じゃんけんスタート";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &JangKeng::button1_Click);
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->panel1->Controls->Add(this->groupBox1);
			this->panel1->Location = System::Drawing::Point(131, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(353, 100);
			this->panel1->TabIndex = 1;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(15, 15);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(318, 64);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label1->Location = System::Drawing::Point(32, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(67, 24);
			this->label1->TabIndex = 0;
			this->label1->Text = L"label1";
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &JangKeng::timer1_Tick);
			// 
			// pictureBox1
			// 
			this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox1->Location = System::Drawing::Point(39, 217);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(82, 60);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pictureBox2->Location = System::Drawing::Point(500, 217);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(82, 60);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 3;
			this->pictureBox2->TabStop = false;
			// 
			// panel_select
			// 
			this->panel_select->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->panel_select->Controls->Add(this->button_paa);
			this->panel_select->Controls->Add(this->button_choki);
			this->panel_select->Controls->Add(this->button_guu);
			this->panel_select->Location = System::Drawing::Point(119, 142);
			this->panel_select->Name = L"panel_select";
			this->panel_select->Size = System::Drawing::Size(380, 59);
			this->panel_select->TabIndex = 4;
			// 
			// button_paa
			// 
			this->button_paa->Location = System::Drawing::Point(252, 16);
			this->button_paa->Name = L"button_paa";
			this->button_paa->Size = System::Drawing::Size(107, 28);
			this->button_paa->TabIndex = 2;
			this->button_paa->Text = L"パー";
			this->button_paa->UseVisualStyleBackColor = true;
			this->button_paa->Click += gcnew System::EventHandler(this, &JangKeng::button_paa_Click);
			// 
			// button_choki
			// 
			this->button_choki->Location = System::Drawing::Point(131, 16);
			this->button_choki->Name = L"button_choki";
			this->button_choki->Size = System::Drawing::Size(106, 28);
			this->button_choki->TabIndex = 1;
			this->button_choki->Text = L"チョキ";
			this->button_choki->UseVisualStyleBackColor = true;
			this->button_choki->Click += gcnew System::EventHandler(this, &JangKeng::button_choki_Click);
			// 
			// button_guu
			// 
			this->button_guu->Location = System::Drawing::Point(23, 16);
			this->button_guu->Name = L"button_guu";
			this->button_guu->Size = System::Drawing::Size(91, 28);
			this->button_guu->TabIndex = 0;
			this->button_guu->Text = L"グー";
			this->button_guu->UseVisualStyleBackColor = true;
			this->button_guu->Click += gcnew System::EventHandler(this, &JangKeng::button_guu_Click);
			// 
			// panel_pre
			// 
			this->panel_pre->BackColor = System::Drawing::Color::Lime;
			this->panel_pre->Controls->Add(this->label2);
			this->panel_pre->Location = System::Drawing::Point(176, 226);
			this->panel_pre->Name = L"panel_pre";
			this->panel_pre->Size = System::Drawing::Size(266, 47);
			this->panel_pre->TabIndex = 5;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label2->Location = System::Drawing::Point(45, 12);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(67, 24);
			this->label2->TabIndex = 0;
			this->label2->Text = L"label2";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(37, 204);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(52, 12);
			this->label3->TabIndex = 6;
			this->label3->Text = L"プレイヤー";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(498, 204);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(67, 12);
			this->label4->TabIndex = 7;
			this->label4->Text = L"コンピューター";
			// 
			// pictureBox3
			// 
			this->pictureBox3->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->pictureBox3->Location = System::Drawing::Point(3, 12);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(30, 26);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox3->TabIndex = 8;
			this->pictureBox3->TabStop = false;
			// 
			// label_player_win
			// 
			this->label_player_win->AutoSize = true;
			this->label_player_win->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label_player_win->Location = System::Drawing::Point(28, 314);
			this->label_player_win->Name = L"label_player_win";
			this->label_player_win->Size = System::Drawing::Size(55, 33);
			this->label_player_win->TabIndex = 9;
			this->label_player_win->Text = L"    ";
			// 
			// label_com_win
			// 
			this->label_com_win->AutoSize = true;
			this->label_com_win->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 24, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label_com_win->Location = System::Drawing::Point(484, 314);
			this->label_com_win->Name = L"label_com_win";
			this->label_com_win->Size = System::Drawing::Size(55, 33);
			this->label_com_win->TabIndex = 10;
			this->label_com_win->Text = L"    ";
			// 
			// button_replay
			// 
			this->button_replay->Location = System::Drawing::Point(540, 3);
			this->button_replay->Name = L"button_replay";
			this->button_replay->Size = System::Drawing::Size(81, 26);
			this->button_replay->TabIndex = 11;
			this->button_replay->Text = L"リプレイ";
			this->button_replay->UseVisualStyleBackColor = true;
			this->button_replay->Click += gcnew System::EventHandler(this, &JangKeng::button_replay_Click);
			// 
			// label_reach_player
			// 
			this->label_reach_player->AutoSize = true;
			this->label_reach_player->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label_reach_player->ForeColor = System::Drawing::Color::Red;
			this->label_reach_player->Location = System::Drawing::Point(30, 281);
			this->label_reach_player->Name = L"label_reach_player";
			this->label_reach_player->Size = System::Drawing::Size(111, 24);
			this->label_reach_player->TabIndex = 12;
			this->label_reach_player->Text = L"リーチ！！";
			this->label_reach_player->Visible = false;
			// 
			// label_reach_computer
			// 
			this->label_reach_computer->AutoSize = true;
			this->label_reach_computer->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(128)));
			this->label_reach_computer->ForeColor = System::Drawing::Color::Red;
			this->label_reach_computer->Location = System::Drawing::Point(491, 280);
			this->label_reach_computer->Name = L"label_reach_computer";
			this->label_reach_computer->Size = System::Drawing::Size(111, 24);
			this->label_reach_computer->TabIndex = 13;
			this->label_reach_computer->Text = L"リーチ！！";
			this->label_reach_computer->Visible = false;
			// 
			// label_hint
			// 
			this->label_hint->AutoSize = true;
			this->label_hint->Location = System::Drawing::Point(173, 314);
			this->label_hint->Name = L"label_hint";
			this->label_hint->Size = System::Drawing::Size(255, 12);
			this->label_hint->TabIndex = 14;
			this->label_hint->Text = L"ヒントヒントヒントヒントヒントヒントヒントヒントヒントヒント";
			this->label_hint->Visible = false;
			// 
			// JangKeng
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(624, 381);
			this->Controls->Add(this->label_hint);
			this->Controls->Add(this->label_reach_computer);
			this->Controls->Add(this->label_reach_player);
			this->Controls->Add(this->button_replay);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->label_com_win);
			this->Controls->Add(this->label_player_win);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->panel_pre);
			this->Controls->Add(this->panel_select);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button1);
			this->Name = L"JangKeng";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"JangKeng";
			this->Load += gcnew System::EventHandler(this, &JangKeng::JangKeng_Load);
			this->panel1->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->panel_select->ResumeLayout(false);
			this->panel_pre->ResumeLayout(false);
			this->panel_pre->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void JangKeng_Load(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void button_guu_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_choki_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_paa_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_replay_Click(System::Object^  sender, System::EventArgs^  e);
};
}
