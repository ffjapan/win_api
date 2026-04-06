#ifndef APP_TASKDATA_H
#define APP_TASKDATA_H

#include <string>
#include <vector>

struct CommentData {
    std::wstring author;
    std::wstring text;
};

struct TaskData {
    int id = 0;
    std::wstring category;
    std::wstring task_name;
    std::wstring person;
    int man_hours = 0;
    std::wstring start;
    std::wstring end;
    std::wstring priority;
    int progress = 0;
    std::wstring status;
    std::vector<CommentData> comments;
};

#endif  // APP_TASKDATA_H
