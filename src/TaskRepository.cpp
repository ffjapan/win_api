#include "TaskRepository.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>

namespace {

std::string WStringToUtf8(const std::wstring& input) {
    if (input.empty()) {
        return {};
    }
    int size_needed = WideCharToMultiByte(
        CP_UTF8, 0, input.c_str(), -1, nullptr, 0, nullptr, nullptr
    );
    if (size_needed <= 0) {
        return {};
    }

    std::string utf8(static_cast<size_t>(size_needed) - 1, '\0');
    WideCharToMultiByte(
        CP_UTF8, 0, input.c_str(), -1, utf8.data(), size_needed, nullptr, nullptr
    );
    return utf8;
}

std::wstring Utf8ToWString(const std::string& input) {
    if (input.empty()) {
        return {};
    }
    int size_needed = MultiByteToWideChar(
        CP_UTF8, 0, input.c_str(), -1, nullptr, 0
    );
    if (size_needed <= 0) {
        return {};
    }

    std::wstring result(static_cast<size_t>(size_needed) - 1, L'\0');
    MultiByteToWideChar(
        CP_UTF8, 0, input.c_str(), -1, result.data(), size_needed
    );
    return result;
}

void Trim(std::wstring& value) {
    size_t start = value.find_first_not_of(L" \t\r\n");
    if (start == std::wstring::npos) {
        value.clear();
        return;
    }
    size_t end = value.find_last_not_of(L" \t\r\n");
    value = value.substr(start, end - start + 1);
}

}  // namespace

bool TaskRepository::LoadTasks(const std::wstring& file_path, std::vector<TaskData>& tasks) {
    std::string path_utf8 = WStringToUtf8(file_path);
    if (path_utf8.empty()) {
        return false;
    }

    std::ifstream file(path_utf8, std::ios::binary);
    if (!file) {
        return false;
    }

    tasks.clear();
    TaskData* current_task = nullptr;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }

        std::wstring wline = Utf8ToWString(line);
        if (wline.empty()) {
            continue;
        }
        if (wline[0] == 0xFEFF) {
            wline.erase(0, 1);
        }
        Trim(wline);
        if (wline.empty()) {
            continue;
        }

        std::wstringstream ss(wline);
        std::wstring type;
        std::getline(ss, type, L',');
        Trim(type);

        if (type == L"T") {
            TaskData task;
            std::wstring tmp;

            std::getline(ss, tmp, L',');
            task.id = std::stoi(tmp);
            std::getline(ss, task.category, L',');
            std::getline(ss, task.task_name, L',');
            std::getline(ss, task.person, L',');
            std::getline(ss, tmp, L',');
            task.man_hours = std::stoi(tmp);
            std::getline(ss, task.start, L',');
            std::getline(ss, task.end, L',');
            std::getline(ss, task.priority, L',');
            std::getline(ss, tmp, L',');
            task.progress = std::stoi(tmp);
            if (!std::getline(ss, task.status)) {
                task.status = L"";
            }

            if (task.status != L"終了") {
                if (task.progress <= 0) {
                    task.status = L"未着手";
                } else if (task.progress >= 100) {
                    task.status = L"完了";
                } else {
                    task.status = L"進行中";
                }
            }

            tasks.push_back(task);
            current_task = &tasks.back();
        } else if (type == L"C" && current_task != nullptr) {
            CommentData c;
            std::getline(ss, c.author, L',');
            std::getline(ss, c.text);
            current_task->comments.push_back(c);
        }
    }

    return true;
}
