#include "StdAfx.h"
#include "stdio.h"
#include "JangKeng.h"


using namespace TobiSagi;

// �Q�[��������
void JangKeng::InitializeStart(void)
{
	this->s_watch = gcnew Diagnostics::Stopwatch();
	this->rnd = gcnew Random();
	this->status = STS_TITLE;
	this->game_cnt = 0;
	this->truemode = false;

	// �{�^���̂ݕ\�����Ă����܂��傩�B
	this->panel1->Visible = false;
	this->panel_select->Visible = false;
	this->panel_pre->Visible = false;
	this->pictureBox3->Visible = false;

	// �C���[�W�摜�����[�h����
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

// �Q�[���X�^�[�g���̏�����
void JangKeng::InitializeGame(void)
{
	cnt_game = 0;
	cnt_win_player = 0;
	cnt_win_com = 0;
	mode = MODE_NOMAL;	// �ŏ��̓m�[�}�����[�h
	aiko_level = 0;
	game_cnt ++;

	// �{�^����R���g���[���[�̕\����ύX����
	this->panel1->Visible = true;
	this->button1->Visible = false;
	this->timer1->Enabled = false;
	this->panel_select->Visible = false;	// ���̓{�^���p�l�����B���B
	this->panel_pre->Visible = false;
	this->pictureBox3->Visible = false;
	this->label_reach_player->Visible = false;
	this->label_reach_computer->Visible = false;
	this->label_hint->Visible = false;
}

// �X�e�[�^�X���聕�J�ڏ���
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
		{	// ���o
			StsGameInputWaitAnime();
		} else {
			// ���̃X�e�[�^�X��
			this->status = STS_GAME_JUDGE;
			StsGameJudge();
		}
		break;

	case STS_GAME_JUDGE:
		// ���傤���Ȃ��̂Łu�r���X������ԁv�������Ō��ɖ߂�
		this->pictureBox1->Image->RotateFlip( RotateFlipType::Rotate270FlipNone );
		this->pictureBox2->Image->RotateFlip( RotateFlipType::Rotate90FlipNone );
		this->panel_pre->Visible = false;	// �|�����p�l�����B���B

		// ����񂯂񔻒���s���B
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
			// �������̏ꍇ
			this->cnt_aiko ++;
			this->status = STS_GAME_PRE_ACTION;
			StsGamePreAction();
			break;
		}
		break;

	case STS_ONE_GAME_WIN:
		// �����ŕ\��������
		this->pictureBox3->Visible = false;
		// ���[�`����
		if( this->cnt_win_player == GAME_REACH_COUNT )
		{	// ���[�`��ʂ�
			this->status = STS_REACH_PLAYER;
			StsReachPlayer();
			// �G���f�B���O����
		} else if( this->cnt_win_player == GAME_WIN_COUNT )
		{	// �G���f�B���O��
			this->status = STS_GAME_TRUE_ENDING;
			StsGameTrueEnding();
		} else {
			this->status = STS_TURN_OPENING;
			StsTurnOpening();
		}
		break;

	case STS_REACH_PLAYER:
		this->pictureBox3->Visible = false;		// �����ŕ\��������
		this->status = STS_TURN_OPENING;
		StsTurnOpening();
		break;

	case STS_ONE_GAME_LOSE:
		this->pictureBox3->Visible = false;		// �����ŕ\��������
		// ���[�`����
		if( this->cnt_win_com == GAME_REACH_COUNT )
		{
			this->status = STS_REACH_COMPUTER;
			StsReachComputer();
		// �Q�[���I�[�o�[����
		} else if( this->cnt_win_com == GAME_WIN_COUNT )
		{	// �R���s���[�^�[�̏����I
			this->status = STS_GAME_OVER;
			StsGameOver();
		} else {	// �܂��B
			this->status = STS_TURN_OPENING;
			StsTurnOpening();
		}
		break;

	case STS_REACH_COMPUTER:
		this->pictureBox3->Visible = false;		// �����ŕ\��������
		this->status = STS_TURN_OPENING;
		StsTurnOpening();
		break;

	default:
		break;
	}
}

// �u�����ڂł��v
void JangKeng::StsTurnOpening(void)
{
	this->cnt_game ++;		// �Q�[�����{�P
	this->cnt_aiko = 0;		// �������J�E���g������

	// �\��������
	System::String ^str;
	str = "�� " + this->cnt_game + " ���A�X�^�[�g�I";
	this->label1->Text = str;	// sprintf_s( "hoge" );
	str = "����= " + this->cnt_win_player;
	this->label_player_win->Text = str;
	str = "����= " + this->cnt_win_com;
	this->label_com_win->Text = str;

	// ���[�`�\������
	if( this->cnt_win_player == GAME_REACH_COUNT )
	{
		this->label_reach_player->Visible = true;
	}
	if( this->cnt_win_com == GAME_REACH_COUNT )
	{
		this->label_reach_computer->Visible = true;
	}

	// �\������
	this->panel1->Visible = true;	// �u�����v�p�l���\��

	// �R���s���[�^�[���[�h�X�V
	RenewComMode();

	// �^�C�}�[�N��
	this->timer1->Interval = 1500;
	this->timer1->Enabled = true;
}

// �u���\�E�w�����[�`���v�R���s���[�^�[���[�h�X�V
void JangKeng::RenewComMode(void)
{
	// �v���C���[�����������Ȃ�A���\���[�h�ɂ���
	if( this->cnt_win_player >= 5 )
	{
		this->mode = MODE_SAGI;	// ���\���[�h

		if( this->cnt_win_com >= 5 )
		{	// �ŏI����I�i�T�u�r�T�j
			this->aiko_level = this->rnd->Next( 5 ) + 5;	// 5�`9��̂������B
		} else {
			this->aiko_level = this->rnd->Next( 3 );		// 0�`2��̂������B
		}
		return;
	}

	// �R���s���[�^�[�����������Ȃ�A�������[�h�ɂ���
	if( this->cnt_win_com >= 5 )
	{
		this->mode = MODE_MAKE;		// �������[�h
		this->aiko_level = this->rnd->Next( 3 ) + 2;	// 2�`4��̂������B
		return;
	}

	// �v���C���[�ŏ��̂Q��́A�����Ȃ菟�����Ă��B
	if( this->cnt_win_player <= 1 )
	{
		this->mode = MODE_MAKE;		// �������[�h
		this->aiko_level = this->rnd->Next( 2 ) + this->cnt_win_com;	// 1��ڂ�0-1��A2��ڂ�1-2��̂������B
		return;
	}

	// �v���C���[���R�����Ă����ԂœG���O����������A�P�񂭂炢�G�ɏ�������B
	if( this->cnt_win_player == 3 && this->cnt_win_com == 0 )
	{
		this->mode = MODE_SAGI;	// ���\���[�h
		this->aiko_level = this->rnd->Next( 2 );	// 0-1��̂������B
		return;
	}

	this->mode = MODE_NOMAL;		// �m�[�}�����[�h
	this->aiko_level = 0;
}

// �u����[��E���[��v
void JangKeng::StsGamePreAction(void)
{
	// �\������
	this->panel1->Visible = false;	// �u�����v�p�l�����B���B
	if( this->cnt_aiko == 0 )
	{
		this->label2->Text = L"����`�񂯁`��I";
		this->timer1->Interval = 1200;
	} else {
		this->label2->Text = L"���`�����`�ŁI";
		this->timer1->Interval = 800;
	}
	this->panel_pre->Visible = true; 

	// �^�C�}�[�N��
	this->timer1->Enabled = true;
}

// �v���C���[���͂܂�
void JangKeng::StsGameInputWait(void)
{
	// �\������
//	this->panel_pre->Visible = false;	// �u����[�񂯁[��v�p�l�����B���B
	this->panel_select->Visible = true;	// ���̓{�^���p�l����\���B

	// �p�����[�^�[�ݒ�
	this->choice_player = TE_NONE;

	// �^�C�}�[�N��
	this->timer1->Interval = 100;	// ������]
	this->timer1->Enabled = true;

	// �A�j���i�H�j�\��
	StsGameInputWaitAnime();
}

// �����_���ŃO�[�E�`���L�E�p�[��\��
void JangKeng::StsGameInputWaitAnime(void)
{
	// �����\�����A��̊p�x���u��v�ɐݒ肷��
	this->pictureBox1->Image = this->SetJangKengHand( PL_PLAYER, this->rnd->Next(3) );
	this->pictureBox2->Image = this->SetJangKengHand( PL_COMPUTER, this->rnd->Next(3) );
}

// ����񂯂�C���[�W�摜��Ԃ�
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

// �W���b�W����
void JangKeng::StsGameJudge(void)
{
	// �\������
	this->panel_select->Visible = false;	// ���̓{�^���p�l�����B���B

	// �|��������
	switch( this->rnd->Next(10) )
	{
	case 0:		this->label2->Text = L"�������I";		break;
	case 1:		this->label2->Text = L"�ǂ����I";		break;
	case 2:		this->label2->Text = L"��������I";		break;
	case 3:		this->label2->Text = L"�E�E�E�H�I";		break;
	case 4:		this->label2->Text = L"���邟�I�I";		break;
	case 5:		this->label2->Text = L"�Ȃ�c���ƁH�I";		break;
	case 6:		this->label2->Text = L"�ۂ���I�I";		break;
	case 7:		this->label2->Text = L"�ǂ���I�I";		break;
	case 8:		this->label2->Text = L"�ق����I�I";		break;
	case 9:		this->label2->Text = L"�ق����I�I";		break;
	}

	// �R���s���[�^�[�̎���v�Z����i���\�j
	this->choice_enemy = GetComJangKeng();

	// �v���C���[�̎�ƃR���s���[�^�[�̎��\��
	this->pictureBox1->Image = this->SetJangKengHand( PL_PLAYER, this->choice_player );
	this->pictureBox1->Image->RotateFlip( RotateFlipType::Rotate90FlipNone );

	this->pictureBox2->Image = this->SetJangKengHand( PL_COMPUTER, this->choice_enemy );
	this->pictureBox2->Image->RotateFlip( RotateFlipType::Rotate270FlipNone );

	// �^�C�}�[�N��
	this->timer1->Interval = 1000;
	this->timer1->Enabled = true;
}

// �R���s���[�^�[�̎���v�Z����i���\�j
int JangKeng::GetComJangKeng(void)
{
	int rt;

	switch( mode )
	{
	case MODE_NOMAL:
		rt = this->rnd->Next(3);	// �����ŃW�����P��
		break;

	case MODE_SAGI:
		if( this->cnt_aiko < this->aiko_level )
		{	// ���������x���񐔂����������ɂ���
			rt = this->choice_player;					// �������ɂ���
		} else {
			// true���[�h����
			if( this->cnt_win_player == 5 && this->cnt_win_com == 5 && this->truemode )
			{
				rt = this->GetLoseChoice( this->choice_player );	// �G���f�B���O�p����
			} else {
				rt = this->GetWinChoice( this->choice_player );	// �R���s���[�^�[�̏����ɂ���
			}
		}
		break;

	case MODE_MAKE:
	default:
		if( this->cnt_aiko < this->aiko_level )
		{	// ���������x���񐔂����������ɂ���
			rt = this->choice_player;					// �������ɂ���
		} else {
			rt = this->GetLoseChoice( this->choice_player );		// �R���s���[�^�[�̕����ɂ���
		}
		break;
	}
	return rt;
}

// ���͂ɑ΂��u���v���Ԃ�
int JangKeng::GetWinChoice( int te )
{
	int rt;

	switch( te )
	{
	case TE_GUU:				rt = TE_PAA;	break;		// �p�[�Ȃ珟�Ă�B
	case TE_CHOKI:				rt = TE_GUU;	break;		// �O�[�Ȃ珟�Ă�B
	case TE_PAA:	default:	rt = TE_CHOKI;	break;		// �`���L�Ȃ珟�Ă�B
	}
	return rt;
}

// ���͂ɑ΂��u������v���Ԃ�
int JangKeng::GetLoseChoice( int te )
{
	int rt;

	switch( te )
	{
	case TE_GUU:				rt = TE_CHOKI;	break;		// �`���L�Ȃ畉����B
	case TE_CHOKI:				rt = TE_PAA;	break;		// �p�[�Ȃ畉����B
	case TE_PAA:	default:	rt = TE_GUU;	break;		// �O�[�Ȃ畉����B
	}
	return rt;
}

// �W�����P���̏��҂����ꂩ��Ԃ��֐�
int JangKeng::CheckJangKengWinner( int pl, int com )
{
	if( pl == TE_GUU )
	{
		if( com == TE_GUU ) return PL_AIKO;			// ������
		if( com == TE_CHOKI ) return PL_PLAYER;		// �v���C���[�̏���
		return PL_COMPUTER;							// �R���s���[�^�[�̏���
	} else if( pl == TE_CHOKI )
	{
		if( com == TE_GUU ) return PL_COMPUTER;		// �R���s���[�^�[�̏���
		if( com == TE_CHOKI ) return PL_AIKO;		// ������
		return PL_PLAYER;							// �v���C���[�̏���
	}
	if( com == TE_GUU ) return PL_PLAYER;			// �v���C���[�̏���
	if( com == TE_CHOKI ) return PL_COMPUTER;		// �R���s���[�^�[�̏���
	return PL_AIKO;									// ������
}

// �v���C���[�̏����I
void JangKeng::StsOneGameWin(void)
{
	this->cnt_win_player ++;	// �v���C���[�������{�P

	// �\������
	this->pictureBox3->BringToFront();			// �R���g���[�����őO�ʂցB
	this->pictureBox1->BringToFront();
	this->pictureBox2->BringToFront();
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_win;
	this->pictureBox3->Visible = true;	

	// �^�C�}�[�N��
	this->timer1->Interval = 2000;
	this->timer1->Enabled = true;
}

// �v���C���[�E���[�`�I
void JangKeng::StsReachPlayer(void)
{
	// �\������
	this->pictureBox3->BringToFront();			// �R���g���[�����őO�ʂցB
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_reach_player;
	this->pictureBox3->Visible = true;	

	// �^�C�}�[�N��
	this->timer1->Interval = 3500;
	this->timer1->Enabled = true;
}

// �R���s���[�^�[�̏����I
void JangKeng::StsOneGameLose(void)
{
	this->cnt_win_com ++;	// �R���s���[�^�[�������{�P

	// �\������
	this->pictureBox3->BringToFront();			// �R���g���[�����őO�ʂցB
	this->pictureBox3->Location = System::Drawing::Point(124, 100);
	this->pictureBox3->Size = System::Drawing::Size(370, 200);
	this->pictureBox3->Image = this->bmp_lose;
	this->pictureBox3->Visible = true;	

	// �^�C�}�[�N��
	this->timer1->Interval = 2000;
	this->timer1->Enabled = true;
}

// �R���s���[�^�[�E���[�`�I
void JangKeng::StsReachComputer(void)
{
	// �\������
	this->pictureBox3->BringToFront();			// �R���g���[�����őO�ʂցB
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_reach_computer;
	this->pictureBox3->Visible = true;	

	// �^�C�}�[�N��
	this->timer1->Interval = 3500;
	this->timer1->Enabled = true;
}

// �Q�[���I�[�o�[
void JangKeng::StsGameOver(void)
{
	// �\������
	this->pictureBox3->BringToFront();			// �R���g���[�����őO�ʂցB
	this->pictureBox3->Location = System::Drawing::Point(140, 95);
	this->pictureBox3->Size = System::Drawing::Size(320, 210);
	this->pictureBox3->Image = this->bmp_gameover;
	this->pictureBox3->Visible = true;	

	// ���X�g���b�Z�[�W�\��
	switch( this->game_cnt )
	{
	case 1:	this->label_hint->Text = L"m9(^�D^)�v�M���[�I�I�I������";			break;
	case 2: this->label_hint->Text = L"�����X�������e�������X������";			break;
	case 3: this->label_hint->Text = L"�A���^���������˂��B���������Ă΂悗��";	break;
	case 4: this->label_hint->Text = L"�c�B�q���g�́u�R�΂R�ł�蒼���v����B";	break;
	default: this->label_hint->Text = L"�������߂Ȃ��Ă��������ʖ��ʖ��ʖ��ʂ����I�I�I"; break;
	}
	this->label_hint->Visible = true;
}

// true�G���f�B���O
void JangKeng::StsGameTrueEnding(void)
{
	// �\������
	this->pictureBox3->BringToFront();			// �R���g���[�����őO�ʂցB
	this->pictureBox3->Location = System::Drawing::Point(10, 50);
	this->pictureBox3->Size = System::Drawing::Size(600, 300);
	this->pictureBox3->Image = this->bmp_true_ending;
	this->pictureBox3->Visible = true;	
	this->label_hint->BringToFront();
	// ���X�g���b�Z�[�W
	this->label_hint->Text = L"���߂łƂ��I�悭�����܂ł��ǂ蒅�����ˁI����ۂǉɂȂ񂾂ˁI�Q�[���N���A�I�I";
	this->label_hint->Visible = true;
}

// �X�^�[�g�{�^������
System::Void JangKeng::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	CheckStatus();
}

// �O�[�{�^��
System::Void JangKeng::button_guu_Click(System::Object^  sender, System::EventArgs^  e)
{
	choice_player = TE_GUU;
	CheckStatus();
}

// �`���L�{�^��
System::Void JangKeng::button_choki_Click(System::Object^  sender, System::EventArgs^  e)
{
	choice_player = TE_CHOKI;
	CheckStatus();
}

// �p�[�{�^��
System::Void JangKeng::button_paa_Click(System::Object^  sender, System::EventArgs^  e)
{
	choice_player = TE_PAA;
	CheckStatus();
}

// �^�C�}�[����
System::Void JangKeng::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
{
	CheckStatus();
}

// ���v���C�{�^��
System::Void JangKeng::button_replay_Click(System::Object^  sender, System::EventArgs^  e)
{	// �����I�ɃQ�[�����Z�b�g�B�ŏ�����n�߂�B
	if( this->cnt_win_player == 3 && this->cnt_win_com == 3 ) 
	{
		this->truemode = true;	// �t���O����
	}
	this->status = STS_TITLE;
	CheckStatus();
}
