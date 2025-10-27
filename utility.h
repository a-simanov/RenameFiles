#pragma once

#include<filesystem>
#include <iostream>

void SearchFile(const std::filesystem::directory_entry& dir_entry, const std::wstring& str);

void DeletePrefix(const std::filesystem::directory_entry& dir_entry, const std::wstring& str);

void AddPrefix(const std::filesystem::directory_entry& dir_entry, const std::wstring& str);
