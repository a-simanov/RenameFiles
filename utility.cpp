#include "utility.h"

using namespace std;

void SearchFile(const filesystem::directory_entry& dir_entry, const wstring& str) {
    wstring old_name = dir_entry.path().wstring();
    if (old_name.find(str) != string::npos) {
        wcout << "File find: " << filesystem::absolute(dir_entry.path()).wstring() << "\n";
    }
}

void DeletePrefix(const filesystem::directory_entry& dir_entry, const wstring& str) {
    wstring old_name = dir_entry.path().wstring();
    if (old_name.find(str) != string::npos) {
        wstring new_name = old_name;
        new_name.erase(old_name.find(str), str.length());
        try {
            filesystem::rename(old_name, new_name);
        } catch (const filesystem::filesystem_error& e) {
            cerr << e.what() << "\n";
        }
    }
}

void AddPrefix(const filesystem::directory_entry& dir_entry, const wstring& str) {
    wstring old_path = dir_entry.path().wstring();
    wstring old_name = dir_entry.path().filename().wstring();
    wstring old_abs = old_path;
    old_abs.erase(old_path.find(old_name));
    wstring new_name = old_abs + str + L"_" + old_name;
    try {
        filesystem::rename(old_path, new_name);
    } catch (const filesystem::filesystem_error& e) {
        cerr << e.what() << "\n";
    }
}
