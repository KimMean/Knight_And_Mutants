#include "Framework.h"

string Utility::ToString(wstring str)
{
    string temp;
    temp.assign(str.begin(), str.end());
    return temp;
}

wstring Utility::ToWString(string str)
{
    wstring temp;
    temp.assign(str.begin(), str.end());
    return temp;
}

vector<string> Utility::SplitString(string origin, const string& tok, const bool& includeLast)
{
    vector<string> result;

    size_t cutAt = 0;

    while ((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }

    if (includeLast && origin.size() > 0)
        result.push_back(origin);

    return result;
}

string Utility::GetFileName(string path)
{
    size_t index = path.find_last_of('/');

    return path.substr(index + 1);
}

wstring Utility::GetFileName(wstring path)
{
    size_t index = path.find_last_of('/');

    return path.substr(index + 1);
}

wstring Utility::GetExtension(wstring filePath)
{
    size_t index = filePath.find_last_of('.');
    
    return filePath.substr(index + 1);
}

wstring Utility::GetFileNameWithoutExtension(wstring file)
{
    wstring fileName = GetFileName(file);

    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

string Utility::GetFileNameWithoutExtension(string file)
{
    string fileName = GetFileName(file);

    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

bool Utility::ExistDirectory(string file)
{
    DWORD fileValue = GetFileAttributesA(file.c_str());

    BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES
        && (fileValue & FILE_ATTRIBUTE_DIRECTORY));

    return temp == TRUE;
}


void Utility::CreateFolders(string file)
{
    vector<string> folders = SplitString(file, "/", false);

    string temp = "";

    for (string folder : folders)
    {
        temp += folder + "/";

        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}
