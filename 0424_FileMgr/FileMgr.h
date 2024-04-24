/**
 * @file FileMgr.h
 * @brief ファイル操作クラス
 * @author Muroya Ian
 * @date 2024/04/25
 */

//===== インクルードガード =====
#pragma once

//===== インクルード部 =====
#include <string>

//===== クラス定義 =====
class FileObject
{
public:

	//コピー＆ムーブ
	FileObject(const FileObject&) = delete;
	FileObject& operator =(const FileObject&) = delete;
	FileObject(FileObject&&) noexcept = delete;
	FileObject& operator=(FileObject&&) noexcept = delete;

	//プロトタイプ宣言
	explicit FileObject() noexcept = default;
	explicit FileObject(const std::string& fileName) noexcept;
	~FileObject() noexcept = default;
	void openFile(const std::string& fileName) noexcept; //ファイル読込
	bool get(char& c) noexcept;                          //文字取得
	void resetItr() noexcept;                            //イテレータをリセット

private:

	//変数宣言
	std::string m_strName;       //ファイル名
	std::string m_strFile;       //ファイルの中身
	std::string::iterator m_itr; //文字取得用
};
