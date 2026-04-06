#ifndef TASK_DATA_H_
#define TASK_DATA_H_

#include <vector>
#include <string>


// ---------------- コメント情報 ----------------
struct Comment{
    std::wstring author;
    std::wstring text;
};

// ---------------- タスク情報 ----------------
struct taskData {
    int id;
    std::wstring category;
    std::wstring task_name;
    std::wstring person;
    int man_hours;
    std::wstring start;
    std::wstring end;
    std::wstring priority;
    int progress;
    std::wstring status;
    std::vector<Comment> comments;
};

struct TaskListContext {
    std::vector<taskData> tasks;
    std::vector<taskData> tasks_filtered;
};

#endif /* TASK_DATA_H_ */
