#ifndef APP_TASKFILTER_H
#define APP_TASKFILTER_H

#include <string>
#include <vector>
#include "TaskData.h"

std::vector<std::wstring> GetUniqueCategories(const std::vector<TaskData>& tasks);
std::vector<std::wstring> GetUniquePersons(const std::vector<TaskData>& tasks);
std::vector<std::wstring> GetUniquePriorities(const std::vector<TaskData>& tasks);
std::vector<std::wstring> GetUniqueStatuses(const std::vector<TaskData>& tasks);

#endif  // APP_TASKFILTER_H
