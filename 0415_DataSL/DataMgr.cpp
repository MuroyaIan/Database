//===========================================
//|�t�@�C����		DataMgr.cpp				|
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

//===== �C���N���[�h�� =====
#include "DataMgr.h"

//===== �N���X���� =====
DATA_INFO::DATA_INFO()
{

}

DATA_INFO::~DATA_INFO()
{

}

DATA_MGR::DATA_MGR(PLAYER* pData)
{
	m_pPlayer = pData;									//�v���C���[�f�[�^�̃|�C���^��ۑ�

	//***** �Z�[�u�f�[�^�̃p�X�ۑ� *****
	lstrcpy(m_Slot[0].m_Path, L"Save_01.data");
	lstrcpy(m_Slot[1].m_Path, L"Save_02.data");
	lstrcpy(m_Slot[2].m_Path, L"Save_03.data");
	lstrcpy(m_State, L"SaveState.data");				//�Z�[�u��ԃf�[�^�̃p�X

	//***** �Z�[�u�f�[�^�̏�ԓǍ� *****
	if (!LoadDataState())
	{
		for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)		//�Z�[�u�f�[�^�̏�ԃ��Z�b�g
			m_bUsed[iCnt] = false;
	}

	//***** �Z�[�u�f�[�^�̏�Ԋm�F *****
	for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)
		lstrcpy(m_Used[iCnt].m_Path, m_bUsed[iCnt] ? L"�f�[�^����" : L"�f�[�^�Ȃ�");
}

DATA_MGR::~DATA_MGR()
{
	//***** �Z�[�u�f�[�^��Ԃ̏��o *****
	SaveDataState();
}

void DATA_MGR::Show()
{
	std::wcout << L"������������������������������������������������������\n\n";
	std::wcout << L"�@�X���b�g�P�F" << m_Used[0].m_Path << L"\n\n";
	std::wcout << L"�@�X���b�g�Q�F" << m_Used[1].m_Path << L"\n\n";
	std::wcout << L"�@�X���b�g�R�F" << m_Used[2].m_Path << L"\n\n";
	std::wcout << L"������������������������������������������������������\n\n";
}

void DATA_MGR::Save(WORD* pSlotNum)
{
	//***** �ϐ��錾 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** ���o���� *****
	pFile->open(m_Slot[*pSlotNum - 1].m_Path, std::ios::binary | std::ios::out);	//�t�@�C�����J��
	if (*pFile)
	{
		pFile->write((char*)m_pPlayer, sizeof(PLAYER));		//�t�@�C�����o��
		pFile->close();										//�t�@�C�������
	}
	else
	{
		std::wcout << L"�t�@�C�����o�͎��s���܂����B\n";	//���o���s
		rewind(stdin);
		getchar();
	}

	//***** �Z�[�u��ԍX�V *****
	m_bUsed[*pSlotNum - 1] = true;
	for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)
		lstrcpy(m_Used[iCnt].m_Path, m_bUsed[iCnt] ? L"�f�[�^����" : L"�f�[�^�Ȃ�");

	//***** ��������� *****
	delete pFile;
	pFile = nullptr;
}

void DATA_MGR::Load(WORD* pSlotNum)
{
	//***** �ϐ��錾 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** �Ǎ����� *****
	pFile->open(m_Slot[*pSlotNum - 1].m_Path, std::ios::binary | std::ios::in);		//�t�@�C�����J��
	if (*pFile)
	{
		pFile->read((char*)m_pPlayer, sizeof(PLAYER));		//�t�@�C���Ǎ���
		pFile->close();										//�t�@�C�������
	}
	else
	{
		std::wcout << L"\n�t�@�C���Ǎ��͎��s���܂����B\n";
		std::wcout << L"[Enter]�Ń^�C�g���֖߂�B";			//�Ǎ����s
		getchar();
	}

	//***** ��������� *****
	delete pFile;
	pFile = nullptr;
}

void DATA_MGR::Delete(WORD* pSlotNum)
{
	//***** �Z�[�u�f�[�^�폜 *****
	_wremove(m_Slot[*pSlotNum - 1].m_Path);

	//***** �Z�[�u��ԍX�V *****
	m_bUsed[*pSlotNum - 1] = false;
	for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)
		lstrcpy(m_Used[iCnt].m_Path, m_bUsed[iCnt] ? L"�f�[�^����" : L"�f�[�^�Ȃ�");
}

void DATA_MGR::SaveDataState()
{
	//***** �ϐ��錾 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** ���o���� *****
	pFile->open(m_State, std::ios::binary | std::ios::out);		//�t�@�C�����J��
	if (*pFile)
	{
		pFile->write((char*)m_bUsed, sizeof(bool) * NUM_SLOT);	//�t�@�C�����o��
		pFile->close();											//�t�@�C�������
	}
	else
	{
		std::wcout << L"�t�@�C�����o�͎��s���܂����B\n";		//���o���s
		rewind(stdin);
		getchar();
	}

	//***** ��������� *****
	delete pFile;
	pFile = nullptr;
}

bool DATA_MGR::LoadDataState()
{
	//***** �ϐ��錾 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** �Ǎ����� *****
	pFile->open(m_State, std::ios::binary | std::ios::in);		//�t�@�C�����J��
	if (*pFile)
	{
		pFile->read((char*)m_bUsed, sizeof(bool) * NUM_SLOT);	//�t�@�C���Ǎ���
		pFile->close();											//�t�@�C�������

		//=== ��������� ===
		delete pFile;
		pFile = nullptr;

		//=== �Ǎ����� ===
		return true;
	}
	else
	{
		//=== ��������� ===
		delete pFile;
		pFile = nullptr;

		//=== �Ǎ����s ===
		return false;
	}
}
