#include <Windows.h>
#include "CsvReader.h"


//コンストラクタ
CsvReader::CsvReader()
{
	data_.clear();
}

//デストラクタ
CsvReader::~CsvReader()
{
	//全データを開放
	for (int y = 0; y < data_.size(); y++)
	{
		for (int x = 0; x < data_[y].size(); x++)
		{
			data_[y][x].clear();
		}
	}
}

//CSVファイルのロード
bool CsvReader::Load(std::string fileName)
{
	//ファイルを開く
	HANDLE hFile;
	hFile = CreateFile(fileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	//開けなかった
	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::string message = "「" + fileName + "」が開けません。\n開いている場合は閉じてください。";
		MessageBox(NULL, message.c_str(), "BaseProjDx9エラー", MB_OK);

		return false;
	}

	//ファイルのサイズ（文字数）を調べる
	DWORD fileSize = GetFileSize(hFile, NULL);

	//すべての文字を入れられる配列を用意
	char* temp = new char[fileSize + 1];

	//ファイルの中身を配列に読み込む
	DWORD dwBytes = 0;
	ReadFile(hFile, temp, fileSize, &dwBytes, NULL);
	temp[fileSize] = '\0';

	//開いたファイルを閉じる
	CloseHandle(hFile);

	//1行のデータを入れる配列
	std::vector<std::string>	line;

	//調べる文字の位置
	DWORD index = 0;

	// 1文字ずつ安全に解析する
	while (index < fileSize)
	{
		std::string val = "";
		bool isLineEnd = false;

		// 1セル分の文字を取得する
		while (index < fileSize)
		{
			char str = temp[index];

			if (str == ',')
			{
				index++; // カンマをスキップ
				break;
			}
			else if (str == '\n')
			{
				index++; // 改行をスキップ
				isLineEnd = true;
				break;
			}
			else if (str == '\r')
			{
				index++;
				// \r の次に \n があればそれも一緒にスキップする
				if (index < fileSize && temp[index] == '\n')
				{
					index++;
				}
				isLineEnd = true;
				break;
			}
			else
			{
				val += str;
				index++;
			}
		}

		// 余計な前後スペースがあれば消すなどの処理（必要に応じて）
		line.push_back(val);

		// 改行に達した、あるいはファイルの最後まで行ったら1行分として確定する
		if (isLineEnd || index >= fileSize)
		{
			// 空行でなければデータに追加
			if (!line.empty() && !(line.size() == 1 && line[0].empty()))
			{
				data_.push_back(line);
			}
			line.clear();
		}
	}

	//読み込んだデータは開放する
	delete[] temp;

	return true;
}

//「,」か「改行」までの文字列を取得
void CsvReader::GetToComma(std::string* result, std::string data, DWORD* index)
{
	//「,」まで一文字ずつresultに入れる
	while (data[*index] != ',' && data[*index] != '\n' && data[*index] != '\r')
	{
		*result += data[*index];
		(*index)++;
	}

	//最後に「\0」を付ける
	*result += '\0';
	(*index)++;
}

//指定した位置のデータを文字列で取得
std::string CsvReader::GetString(DWORD x, DWORD y)
{
	if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight())
		return "";

	return data_[y][x];
}

//指定した位置のデータを整数で取得
int CsvReader::GetIntValue(DWORD x, DWORD y)
{
	return atoi(GetString(x, y).c_str());
}

float CsvReader::GetFloatValue(DWORD x, DWORD y)
{
	return std::stof(GetString(x, y).c_str());
}

//ファイルの列数を取得
size_t CsvReader::GetWidth()
{
	return data_[0].size();
}

//ファイルの行数を取得
size_t CsvReader::GetHeight()
{
	return data_.size();
}