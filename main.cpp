#include <filesystem>
#include <iostream>
#include <string>
#include <functional>



#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #include <fcntl.h>
#elif __linux__
    #include <locale>
    #include <unistd.h>
#endif

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

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

using Func = function<void(const filesystem::directory_entry& dir_entry, const wstring& str)>;

void DirectoryTraversal (const string& exec, const path& p, const filesystem::file_status& status, const wstring& str, Func& func) {
    if (status.type() == filesystem::file_type::directory) {
        for (const auto& dir_entry: filesystem::directory_iterator(p)) {
            if (dir_entry.status().type() == filesystem::file_type::directory) {
                DirectoryTraversal(exec, dir_entry.path(), status, str, func);
            } else {
                if (dir_entry.path().filename().string() == exec) continue;
                func(dir_entry, str);
            }
        }
    }
}

int main(int argc, char** argv) {
    #if defined(WIN32) || defined(WIN64)
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stdin),  _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
    #elif defined(linux)
        setlocale(LC_ALL, "ru_RU.UTF-8");
    #endif

    wstring response;
    wcout << "If you want find file enter 1\n" <<
        "If you want delete prefix in files name enter 2\n" <<
        "If you want add prefix in all files name enter 3\n";
    while (wcin) {
        wcin >> response;
        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        const path& p = buffer;
        string exec_path = *argv;
        string exec_file = exec_path.substr(exec_path.find_last_of("\\") + 1);
        Func func;
        wstring response_str;
        if (response == L"1") {
            wcout << "Enter string to search: ";
            func = SearchFile;
            wcout << "\n";
        } else if (response == L"2") {
            wcout << "Enter string to delete: ";
            func = DeletePrefix;
            wcout << "\n";
        } else if (response == L"3") {
            wcout << "Enter prefix to add: ";
            func = AddPrefix;
            wcout << "\n";
        }
        wcin >> response_str;
        DirectoryTraversal(exec_file, p, filesystem::status(p), response_str, func);
    }
}
