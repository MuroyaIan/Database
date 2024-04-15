//===========================================
//|ファイル名		DataMgr.cpp				|
//|-----------------------------------------|
//|プログラム概要	データ入出力の処理		|
//|-----------------------------------------|
//|科目担当			佐々木 遼太 先生		|
//|-----------------------------------------|
//|クラス記号		AT_12D_192				|
//|出席番号			47						|
//|学籍番号			00616					|
//|制作者			室谷 イアン				|
//|-----------------------------------------|
//|担任				水貝 純	先生			|
//===========================================

//===== インクルード部 =====
#include "DataMgr.h"

//===== クラス実装 =====
DATA_INFO::DATA_INFO()
{

}

DATA_INFO::~DATA_INFO()
{

}

DATA_MGR::DATA_MGR(PLAYER* pData)
{
	m_pPlayer = pData;									//プレイヤーデータのポインタを保存

	//***** セーブデータのパス保存 *****
	lstrcpy(m_Slot[0].m_Path, L"Save_01.data");
	lstrcpy(m_Slot[1].m_Path, L"Save_02.data");
	lstrcpy(m_Slot[2].m_Path, L"Save_03.data");
	lstrcpy(m_State, L"SaveState.data");				//セーブ状態データのパス

	//***** セーブデータの状態読込 *****
	if (!LoadDataState())
	{
		for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)		//セーブデータの状態リセット
			m_bUsed[iCnt] = false;
	}

	//***** セーブデータの状態確認 *****
	for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)
		lstrcpy(m_Used[iCnt].m_Path, m_bUsed[iCnt] ? L"データあり" : L"データなし");
}

DATA_MGR::~DATA_MGR()
{
	//***** セーブデータ状態の書出 *****
	SaveDataState();
}

void DATA_MGR::Show()
{
	std::wcout << L"━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
	std::wcout << L"　スロット１：" << m_Used[0].m_Path << L"\n\n";
	std::wcout << L"　スロット２：" << m_Used[1].m_Path << L"\n\n";
	std::wcout << L"　スロット３：" << m_Used[2].m_Path << L"\n\n";
	std::wcout << L"━━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n";
}

void DATA_MGR::Save(WORD* pSlotNum)
{
	//***** 変数宣言 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** 書出処理 *****
	pFile->open(m_Slot[*pSlotNum - 1].m_Path, std::ios::binary | std::ios::out);	//ファイルを開く
	if (*pFile)
	{
		pFile->write((char*)m_pPlayer, sizeof(PLAYER));		//ファイル書出し
		pFile->close();										//ファイルを閉じる
	}
	else
	{
		std::wcout << L"ファイル書出は失敗しました。\n";	//書出失敗
		rewind(stdin);
		getchar();
	}

	//***** セーブ状態更新 *****
	m_bUsed[*pSlotNum - 1] = true;
	for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)
		lstrcpy(m_Used[iCnt].m_Path, m_bUsed[iCnt] ? L"データあり" : L"データなし");

	//***** メモリ解放 *****
	delete pFile;
	pFile = nullptr;
}

void DATA_MGR::Load(WORD* pSlotNum)
{
	//***** 変数宣言 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** 読込処理 *****
	pFile->open(m_Slot[*pSlotNum - 1].m_Path, std::ios::binary | std::ios::in);		//ファイルを開く
	if (*pFile)
	{
		pFile->read((char*)m_pPlayer, sizeof(PLAYER));		//ファイル読込み
		pFile->close();										//ファイルを閉じる
	}
	else
	{
		std::wcout << L"\nファイル読込は失敗しました。\n";
		std::wcout << L"[Enter]でタイトルへ戻る。";			//読込失敗
		getchar();
	}

	//***** メモリ解放 *****
	delete pFile;
	pFile = nullptr;
}

void DATA_MGR::Delete(WORD* pSlotNum)
{
	//***** セーブデータ削除 *****
	_wremove(m_Slot[*pSlotNum - 1].m_Path);

	//***** セーブ状態更新 *****
	m_bUsed[*pSlotNum - 1] = false;
	for (int iCnt = 0; iCnt < NUM_SLOT; iCnt++)
		lstrcpy(m_Used[iCnt].m_Path, m_bUsed[iCnt] ? L"データあり" : L"データなし");
}

void DATA_MGR::SaveDataState()
{
	//***** 変数宣言 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** 書出処理 *****
	pFile->open(m_State, std::ios::binary | std::ios::out);		//ファイルを開く
	if (*pFile)
	{
		pFile->write((char*)m_bUsed, sizeof(bool) * NUM_SLOT);	//ファイル書出し
		pFile->close();											//ファイルを閉じる
	}
	else
	{
		std::wcout << L"ファイル書出は失敗しました。\n";		//書出失敗
		rewind(stdin);
		getchar();
	}

	//***** メモリ解放 *****
	delete pFile;
	pFile = nullptr;
}

bool DATA_MGR::LoadDataState()
{
	//***** 変数宣言 *****
	std::fstream* pFile = DBG_NEW std::fstream;

	//***** 読込処理 *****
	pFile->open(m_State, std::ios::binary | std::ios::in);		//ファイルを開く
	if (*pFile)
	{
		pFile->read((char*)m_bUsed, sizeof(bool) * NUM_SLOT);	//ファイル読込み
		pFile->close();											//ファイルを閉じる

		//=== メモリ解放 ===
		delete pFile;
		pFile = nullptr;

		//=== 読込成功 ===
		return true;
	}
	else
	{
		//=== メモリ解放 ===
		delete pFile;
		pFile = nullptr;

		//=== 読込失敗 ===
		return false;
	}
}
