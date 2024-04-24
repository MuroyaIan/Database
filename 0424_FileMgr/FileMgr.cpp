//===== インクルード部 =====
#include  "FileMgr.h"
#include <fstream>

//===== クラス実装 =====
FileObject::FileObject(const std::string& fileName) noexcept
{
	//ファイル読込
	openFile(fileName);
}

/**
 * ファイル読込
 *
 * \param fileName
 * \return void
 */
void FileObject::openFile(const std::string& fileName) noexcept
{
	//ファイル名登録
	m_strName = fileName;
	m_strFile.clear();

	//ファイルの中身を取得
	std::ifstream file(m_strName);
	char c{'\0'};
	while (file.get(c))
		m_strFile += c;
	file.close();

	//イテレータ初期化
	m_itr = m_strFile.begin();
}

/**
 * 文字取得
 *
 * \param c
 * \return bool
 */
bool FileObject::get(char& c) noexcept
{
	//末尾まで１文字ずつ取り出す
	if (m_itr != m_strFile.end())
	{
		c = *m_itr;
		++m_itr;
		return true;
	}

	//取り出す終了
	return false;
}

/**
 * イテレータをリセット
 *
 * \param
 * \return void
 */
void FileObject::resetItr() noexcept
{
	m_itr = m_strFile.begin();
}
