#include "utility.h""
#include <filesystem>
#include <iostream>
#include <string>
#include <functional>

#if defined(_WIN32) || (_WIN64)
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #include <fcntl.h>
#elif defined(linux)
    #include <unistd.h>
#endif

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
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
    string exec_path = *argv;
    string exec_file = exec_path.substr(exec_path.find_last_of("\\") + 1);
    path p = exec_path.erase(exec_path.find_last_of("\\") + 1);

    #if defined(_WIN32) || (_WIN64)
        _setmode(_fileno(stdout), _O_U16TEXT);
        _setmode(_fileno(stdin),  _O_U16TEXT);
        _setmode(_fileno(stderr), _O_U16TEXT);
    #elif defined(linux)
        setlocale(LC_ALL, "ru_RU.UTF-8");
        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        p = buffer;
    #endif

    wstring response;
    wcout << "If you want find file enter 1\n" <<
        "If you want delete prefix in files name enter 2\n" <<
        "If you want add prefix in all files name enter 3\n";
    while (true) {
        wcin >> response;
        Func func;
        wstring response_str;
        if (response == L"1") {
            wcout << L"Enter string to search: ";
            func = SearchFile;
            wcout << "\n";
        } else if (response == L"2") {
            wcout << L"Enter string to delete: ";
            func = DeletePrefix;
            wcout << "\n";
        } else if (response == L"3") {
            wcout << L"Enter prefix to add: ";
            func = AddPrefix;
            wcout << "\n";
        } else {
            cerr << "Unknown command\n";
            break;
        }
        wcin >> response_str;
        DirectoryTraversal(exec_file, p, filesystem::status(p), response_str, func);
    }
}
