#pragma once


namespace Utility
{
	string ToString(wstring str);
	wstring ToWString(string str);

	vector<string> SplitString(string origin, const string& tok, const bool& includeLast);

	string GetFileName(string path);
	wstring GetFileName(wstring path);

	wstring GetExtension(wstring filePath);

	wstring GetFileNameWithoutExtension(wstring file);
	string GetFileNameWithoutExtension(string file);

	bool ExistDirectory(string file);

	void CreateFolders(string file);
}