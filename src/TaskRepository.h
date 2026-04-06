#ifndef APP_TASKREPOSITORY_H
#define APP_TASKREPOSITORY_H

#include <string>
#include <vector>
#include "TaskData.h"

class TaskRepository {
    public:
        static std::wstring ResolveTaskFilePath();
        static bool LoadTasks(const std::wstring& file_path, std::vector<TaskData>& tasks);
};

#endif  // APP_TASKREPOSITORY_H
