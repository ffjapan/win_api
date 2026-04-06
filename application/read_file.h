#ifndef READ_FILE_H_
#define READ_FILE_H_

#include "task_data.h"
#include <vector>

void loadTasks(const wchar_t* filename, std::vector<taskData> &taskData);

#endif /* READ_FILE_H_ */
