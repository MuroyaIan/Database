//===========================================
//|�t�@�C����		DataMgr.h				|
//|-----------------------------------------|
//|�v���O�����T�v	�f�[�^���o�͂̏���		|
//|-----------------------------------------|
//|�ȖڒS��			���X�� �ɑ� �搶		|
//|-----------------------------------------|
//|�N���X�L��		AT_12D_192				|
//|�o�Ȕԍ�			47						|
//|�w�Дԍ�			00616					|
//|�����			���J �C�A��				|
//|-----------------------------------------|
//|�S�C				���L ��	�搶			|
//===========================================

//===== �C���N���[�h�K�[�h =====
#pragma once

//===== �C���N���[�h�� =====
#include "Player.h"

//===== �萔�E�}�N����` =====
#define LEN_DATA_NAME	(32)	//�f�[�^�t�@�C�����̍ő啶����
#define NUM_SLOT		(3)		//�f�[�^�X���b�g�̐�

//===== �N���X��` =====
class DATA_INFO
{
public:
	//* �ϐ��錾 *
	WCHAR m_Path[LEN_DATA_NAME];	//�f�[�^�t�@�C����
	//* �R���X�g���N�^ *
	DATA_INFO();
	~DATA_INFO();
};

class DATA_MGR
{
	//* �ϐ��錾 *
	DATA_INFO m_Slot[NUM_SLOT];		//�f�[�^�X���b�g�̔z��
	WCHAR m_State[LEN_DATA_NAME];	//�Z�[�u��ԃf�[�^�̃t�@�C����
	bool m_bUsed[NUM_SLOT];			//�X���b�g�g�p���
	DATA_INFO m_Used[NUM_SLOT];		//�X���b�g�g�p��Ԃ̕\����
	PLAYER* m_pPlayer;				//�R���|�W�V����
	//* �v���g�^�C�v�錾 *
	void SaveDataState();			//�Z�[�u��ԃf�[�^�̏��o
	bool LoadDataState();			//�Z�[�u��ԃf�[�^�̓Ǎ�
public:
	//* �R���X�g���N�^ *
	DATA_MGR(PLAYER*);
	~DATA_MGR();
	//* �v���g�^�C�v�錾 *
	void Show();					//�Z�[�u�f�[�^�̕\��
	void Save(WORD*);				//�Z�[�u�f�[�^�̏��o
	void Load(WORD*);				//�Z�[�u�f�[�^�̓Ǎ�
	void Delete(WORD*);				//�Z�[�u�f�[�^�̍폜
};
