#include "TaskFilter.h"

#include <set>

namespace {

std::vector<std::wstring> ToVector(const std::set<std::wstring>& values) {
    return std::vector<std::wstring>(values.begin(), values.end());
}

}  // namespace

std::vector<std::wstring> GetUniqueCategories(const std::vector<TaskData>& tasks) {
    std::set<std::wstring> values;
    for (const auto& task : tasks) {
        values.insert(task.category);
    }
    return ToVector(values);
}

std::vector<std::wstring> GetUniquePersons(const std::vector<TaskData>& tasks) {
    std::set<std::wstring> values;
    for (const auto& task : tasks) {
        values.insert(task.person);
    }
    return ToVector(values);
}

std::vector<std::wstring> GetUniquePriorities(const std::vector<TaskData>& tasks) {
    std::set<std::wstring> values;
    for (const auto& task : tasks) {
        values.insert(task.priority);
    }
    return ToVector(values);
}

std::vector<std::wstring> GetUniqueStatuses(const std::vector<TaskData>& tasks) {
    std::set<std::wstring> values;
    for (const auto& task : tasks) {
        values.insert(task.status);
    }
    return ToVector(values);
}
