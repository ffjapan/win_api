#ifndef APP_TASKVIEWER_H
#define APP_TASKVIEWER_H

#include "MyWinAPI/Screen.h"
#include "MyWinAPI/Button.h"
#include "MyWinAPI/PullDown.h"
#include "MyWinAPI/Label.h"
#include "MyWinAPI/ListView.h"
#include "TaskData.h"
#include <vector>

class TaskViewer : public Screen {
    public:
        Button btn_to_login;
        Button btn_register;
        Button btn_delete;
        PullDown combo_category;
        PullDown combo_person;
        PullDown combo_priority;
        PullDown combo_status;
        Label label_category;
        Label label_person;
        Label label_priority;
        Label label_status;
        ListView task_list;

        ~TaskViewer();
    protected:
        LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
        void onResize(int w, int h) override;

    private:
        HFONT font_ = nullptr;
        std::vector<TaskData> tasks_;
        std::vector<TaskData> filtered_tasks_;

        void SetupFont();
        void FillComboBoxWithAll(PullDown& pull_down, const std::vector<std::wstring>& items);
        void ApplyFilter();
        bool HandleHeaderNotify(LPNMHDR pnmh);
        bool HandleItemActivate(LPNMHDR pnmh);
        LRESULT HandleCustomDraw(LPNMHDR pnmh);
        void DrawProgressBar(HDC hdc, const RECT& rc_cell, int progress);
};

#endif  // APP_TASKVIEWER_H
