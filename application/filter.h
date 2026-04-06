/*
 * filter.h
 *
 *  Created on: 2026/04/02
 *      Author: midry
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <string>
#include <windows.h>
#include <vector>
#include <set>
#include "task_data.h"

template<typename T>
std::vector<std::wstring> getUniqueValues(const std::vector<taskData>& tasks, T taskData::* field) {
    std::set<std::wstring> s;
    for (auto& t : tasks) s.insert(t.*field);
    return std::vector<std::wstring>(s.begin(), s.end());
}
void setComboItems(HWND hCombo, const std::vector<std::wstring>& items);

#endif /* FILTER_H_ */
