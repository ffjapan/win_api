#define UNICODE
#define _UNICODE
#include <windows.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <locale>
#include <codecvt>

#include "task_data.h"


void loadTasks(const wchar_t* filename, std::vector<taskData>& tasks) {
    // --- wchar_t → UTF-8（ファイルパス用） ---
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, filename, -1, NULL, 0, NULL, NULL);
    std::string filename_utf8(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, filename, -1, &filename_utf8[0], size_needed, NULL, NULL);

    // --- ファイルオープン ---
    std::ifstream file(filename_utf8, std::ios::binary);
    if (!file) {
        MessageBoxW(NULL, L"ファイル開けない", L"", MB_OK);
        return;
    }

//    MessageBoxW(NULL, L"ファイル開けた", L"", MB_OK);

    // --- UTF-8 → wstring変換 ---
    auto utf8_to_wstring = [](const std::string& str) -> std::wstring {
        int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
        std::wstring wstr(size, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
        wstr.pop_back(); // null削除
        return wstr;
    };

    tasks.clear();

    std::string line;
    taskData* currentTask = nullptr;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        // --- UTF-8 → wstring ---
        std::wstring wline = utf8_to_wstring(line);

        // --- BOM除去 ---
        if (!wline.empty() && wline[0] == 0xFEFF) {
            wline.erase(0, 1);
        }

        // --- 前後の空白・改行除去 ---
        wline.erase(0, wline.find_first_not_of(L" \t\r\n"));
        wline.erase(wline.find_last_not_of(L" \t\r\n") + 1);

        // --- デバッグ用 ---
//        MessageBoxW(NULL, wline.c_str(), L"1行", MB_OK);

        std::wstringstream ss(wline);
        std::wstring type;
        std::getline(ss, type, L',');

        // --- typeトリム ---
        type.erase(0, type.find_first_not_of(L" \t\r\n"));
        type.erase(type.find_last_not_of(L" \t\r\n") + 1);

//        MessageBoxW(NULL, type.c_str(), L"type", MB_OK);

        // ---タスク行---
        if (type == L"T") {
            taskData t;
            std::wstring tmp;

            std::getline(ss, tmp, L','); t.id = std::stoi(tmp);
            std::getline(ss, t.category, L',');
            std::getline(ss, t.task_name, L',');
            std::getline(ss, t.person, L',');
            std::getline(ss, tmp, L','); t.man_hours = std::stoi(tmp);
            std::getline(ss, t.start, L',');
            std::getline(ss, t.end, L',');
            std::getline(ss, t.priority, L',');
            std::getline(ss, tmp, L','); t.progress = std::stoi(tmp);

            //---ステータス決定---
            if (!std::getline(ss, t.status)) {
                t.status = L"";  // 読み取れなかった場合は空に
            }

            if (t.status != L"終了") {
                if (t.progress == 0) {
                    t.status = L"未着手";
                }
                else if (t.progress == 100) {
                    t.status = L"完了";
                }
                else {
                    t.status = L"進行中";
                }
            }

            tasks.push_back(t);
            currentTask = &tasks.back();
        }

        // ---コメント行---
        else if (type == L"C" && currentTask != nullptr) {
            Comment c;
            std::getline(ss, c.author, L',');
            std::getline(ss, c.text);
            currentTask->comments.push_back(c);
        }

        // 不正行
//        else {
//            MessageBoxW(NULL, wline.c_str(), L"無視された行", MB_OK);
//        }
    }

    // --- デバッグ行数 ---
//    MessageBoxW(NULL, std::to_wstring(tasks.size()).c_str(), L"タスク数", MB_OK);
}
