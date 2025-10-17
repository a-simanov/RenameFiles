#include <filesystem>
#include <iostream>
#include <string>
#include <functional>

#if defined(WIN32)
    #include <direct.h>
#elif defined(WIN64)
    #include <direct.h>
#elif defined(linux)
    #include <unistd.h>
#endif

using namespace std;
using filesystem::path;

path operator""_p(const char* data, std::size_t sz) {
    return path(data, data + sz);
}

void SearchFile(const filesystem::directory_entry& dir_entry, const string& str) {
    string old_name = dir_entry.path().string();
    if (old_name.find(str) != string::npos) {
        cout << "File find: " << filesystem::absolute(dir_entry.path()) ;
    }
}

void DeletePrefix(const filesystem::directory_entry& dir_entry, const string& str) {
    string old_name = dir_entry.path().string();
    if (old_name.find(str) != string::npos) {
        string new_name = old_name;
        new_name.erase(old_name.find(str), str.length());
        try {
            filesystem::rename(old_name, new_name);
        } catch (const filesystem::filesystem_error& e) {
            cerr << "Can't rename file " << old_name << " " << e.what();
        }
    }
}

void AddPrefix(const filesystem::directory_entry& dir_entry, const string& str) {
    string old_path = dir_entry.path().string();
    string old_name = dir_entry.path().filename().string();
    string old_abs = old_path;
    old_abs.erase(old_path.find(old_name));
    string new_name = old_abs + str + "_" + old_name;
    try {
        filesystem::rename(old_path, new_name);
    } catch (const filesystem::filesystem_error& e) {
        cerr << "Can't rename file " << old_name << " " << e.what();
    }
}

using Func = function<void(const filesystem::directory_entry& dir_entry, const string& str)>;

void DirectoryTraversal (const string& exec, const path& p, const filesystem::file_status& status, const string& str, Func& func) {
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
    string response;

    cout << "If you want find file enter 1\n" <<
        "If you want delete prefix in files name enter 2\n" <<
        "If you want add prefix in all files name enter 3\n";
    const size_t size = 1024;
    char buffer[size];
    getcwd(buffer, size);
    const path& p = buffer;
    string path = p.filename().string();
    string exec_path = *argv;
    string exec_file = exec_path.substr(exec_path.find_last_of("\\") + 1);
    Func func;
    while(cin >> response) {
        string response_str;
        if (response == "1") {
            func = SearchFile;
        } else if (response == "2") {
            func = DeletePrefix;
        } else if (response == "3") {
            func = AddPrefix;
        }
        cin >> response_str;
        DirectoryTraversal(exec_file, p, filesystem::status(p), response_str, func);
    }
}
