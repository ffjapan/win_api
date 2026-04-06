#include "TaskViewer.h"

#include "Message.h"
#include "TaskFilter.h"
#include "TaskRepository.h"
#include <commctrl.h>
#include <windows.h>
#include <string>

namespace {

enum TaskViewerControlId {
    ID_BTN_TO_LOGIN = 101,
    ID_BTN_REGISTER = 102,
    ID_BTN_DELETE = 103,
    ID_COMBO_CATEGORY = 201,
    ID_COMBO_PERSON = 202,
    ID_COMBO_PRIORITY = 203,
    ID_COMBO_STATUS = 204,
    ID_TASK_LIST = 301
};

enum TaskListColumn {
    COL_CATEGORY = 0,
    COL_TASKNAME = 1,
    COL_PERSON = 2,
    COL_START = 3,
    COL_END = 4,
    COL_PRIORITY = 5,
    COL_PROGRESS = 6,
    COL_STATUS = 7,
    COL_NUM = 8
};

constexpr int kButtonWidth = 100;
constexpr int kButtonHeight = 28;
constexpr int kLabelWidth = 120;
constexpr int kLabelHeight = 20;
constexpr int kComboWidth = 120;
constexpr int kComboHeight = 300;

}  // namespace

TaskViewer::~TaskViewer() {
    if (font_ != nullptr) {
        DeleteObject(font_);
        font_ = nullptr;
    }
}

void TaskViewer::SetupFont() {
    if (font_ != nullptr) {
        DeleteObject(font_);
    }

    font_ = CreateFontW(
        -14, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Meiryo UI"
    );

    if (font_ == nullptr) {
        return;
    }

    HWND controls[] = {
        task_list.Handle(),
        combo_category.Handle(),
        combo_person.Handle(),
        combo_priority.Handle(),
        combo_status.Handle(),
        btn_to_login.Handle(),
        btn_register.Handle(),
        btn_delete.Handle(),
        label_category.Handle(),
        label_person.Handle(),
        label_priority.Handle(),
        label_status.Handle()
    };

    for (HWND h : controls) {
        if (h != nullptr) {
            SendMessageW(h, WM_SETFONT, (WPARAM)font_, TRUE);
        }
    }
}

void TaskViewer::FillComboBoxWithAll(PullDown& pull_down, const std::vector<std::wstring>& items) {
    pull_down.ClearItems();
    pull_down.AddItem(L"ALL");
    for (const auto& item : items) {
        pull_down.AddItem(item);
    }
    pull_down.SetSelectedIndex(0);
}

void TaskViewer::ApplyFilter() {
    filtered_tasks_.clear();

    const std::wstring filter_category = combo_category.GetSelectedText();
    const std::wstring filter_person = combo_person.GetSelectedText();
    const std::wstring filter_priority = combo_priority.GetSelectedText();
    const std::wstring filter_status = combo_status.GetSelectedText();

    for (const auto& task : tasks_) {
        if ((filter_category != L"ALL" && !filter_category.empty() && task.category != filter_category) ||
            (filter_person != L"ALL" && !filter_person.empty() && task.person != filter_person) ||
            (filter_priority != L"ALL" && !filter_priority.empty() && task.priority != filter_priority) ||
            (filter_status != L"ALL" && !filter_status.empty() && task.status != filter_status)) {
            continue;
        }
        filtered_tasks_.push_back(task);
    }

    task_list.DeleteAllItems();
    for (int row = 0; row < static_cast<int>(filtered_tasks_.size()); ++row) {
        const auto& task = filtered_tasks_[row];
        task_list.InsertItem(row, task.task_name);
        task_list.SetItemText(row, COL_CATEGORY, task.category);
        task_list.SetItemText(row, COL_TASKNAME, task.task_name);
        task_list.SetItemText(row, COL_PERSON, task.person);
        task_list.SetItemText(row, COL_START, task.start);
        task_list.SetItemText(row, COL_END, task.end);
        task_list.SetItemText(row, COL_PRIORITY, task.priority);
        task_list.SetItemText(row, COL_PROGRESS, std::to_wstring(task.progress));
        task_list.SetItemText(row, COL_STATUS, task.status);
    }
}

bool TaskViewer::HandleHeaderNotify(LPNMHDR pnmh) {
    HWND h_header = task_list.HeaderHandle();
    if (h_header == nullptr || pnmh->hwndFrom != h_header) {
        return false;
    }

    LONG style = GetWindowLong(h_header, GWL_STYLE);
    style &= ~HDS_BUTTONS;
    SetWindowLong(h_header, GWL_STYLE, style);

    switch (pnmh->code) {
        case HDN_BEGINTRACKA:
        case HDN_BEGINTRACKW:
        case HDN_ITEMCHANGINGA:
        case HDN_ITEMCHANGINGW:
            return true;
    }
    return false;
}

bool TaskViewer::HandleItemActivate(LPNMHDR pnmh) {
    if (pnmh->hwndFrom != task_list.Handle() || pnmh->code != NM_DBLCLK) {
        return false;
    }

    auto* item = reinterpret_cast<LPNMITEMACTIVATE>(pnmh);
    if (item->iItem < 0 || item->iSubItem != COL_TASKNAME ||
        item->iItem >= static_cast<int>(filtered_tasks_.size())) {
        return false;
    }

    const auto& task = filtered_tasks_[item->iItem];
    std::wstring msg =
        L"タスク名: " + task.task_name + L"\n" +
        L"分類: " + task.category + L"\n" +
        L"担当者: " + task.person + L"\n" +
        L"開始日: " + task.start + L"\n" +
        L"終了日: " + task.end + L"\n" +
        L"優先度: " + task.priority + L"\n" +
        L"進捗: " + std::to_wstring(task.progress) + L"%\n" +
        L"ステータス: " + task.status;

    MessageBoxW(Handle(), msg.c_str(), L"タスク詳細", MB_OK);
    return true;
}

void TaskViewer::DrawProgressBar(HDC hdc, const RECT& rc_cell, int progress) {
    FillRect(hdc, &rc_cell, (HBRUSH)(COLOR_WINDOW + 1));

    int safe_progress = progress;
    if (safe_progress < 0) {
        safe_progress = 0;
    }
    if (safe_progress > 100) {
        safe_progress = 100;
    }

    const int padding = 2;
    const int text_width = 40;

    RECT rc_bar = rc_cell;
    rc_bar.left += padding;
    rc_bar.top += padding;
    rc_bar.bottom -= padding;
    rc_bar.right -= text_width;

    HBRUSH h_bg = CreateSolidBrush(RGB(220, 220, 220));
    FillRect(hdc, &rc_bar, h_bg);
    DeleteObject(h_bg);

    int bar_width = rc_bar.right - rc_bar.left;
    int progress_width = (safe_progress * bar_width) / 100;
    RECT rc_done = rc_bar;
    rc_done.right = rc_done.left + progress_width;

    HBRUSH h_done = CreateSolidBrush(RGB(255, 140, 0));
    FillRect(hdc, &rc_done, h_done);
    DeleteObject(h_done);

    std::wstring text = std::to_wstring(safe_progress) + L"%";
    RECT rc_text = rc_cell;
    rc_text.left = rc_bar.right;

    SetBkMode(hdc, TRANSPARENT);
    DrawTextW(hdc, text.c_str(), -1, &rc_text, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
}

LRESULT TaskViewer::HandleCustomDraw(LPNMHDR pnmh) {
    if (pnmh->hwndFrom != task_list.Handle() || pnmh->code != NM_CUSTOMDRAW) {
        return CDRF_DODEFAULT;
    }

    auto* custom_draw = reinterpret_cast<LPNMLVCUSTOMDRAW>(pnmh);
    switch (custom_draw->nmcd.dwDrawStage) {
        case CDDS_PREPAINT:
            return CDRF_NOTIFYITEMDRAW;
        case CDDS_ITEMPREPAINT:
            return CDRF_NOTIFYSUBITEMDRAW;
        case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
            int row = static_cast<int>(custom_draw->nmcd.dwItemSpec);
            int col = custom_draw->iSubItem;

            if (row < 0 || row >= static_cast<int>(filtered_tasks_.size())) {
                return CDRF_DODEFAULT;
            }

            custom_draw->clrText = RGB(0, 0, 0);
            if (col == COL_TASKNAME) {
                custom_draw->clrText = RGB(0, 102, 204);
            }

            if (col == COL_PROGRESS) {
                RECT rc_cell;
                ListView_GetSubItemRect(task_list.Handle(), row, col, LVIR_BOUNDS, &rc_cell);
                DrawProgressBar(custom_draw->nmcd.hdc, rc_cell, filtered_tasks_[row].progress);
                return CDRF_SKIPDEFAULT;
            }
            return CDRF_DODEFAULT;
        }
    }
    return CDRF_DODEFAULT;
}

LRESULT TaskViewer::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            ListView::InitCommonControls();

            btn_to_login.Create(Handle(), L"ログイン画面へ", 10, 10, ID_BTN_TO_LOGIN);
            btn_register.Create(Handle(), L"登録", 120, 10, ID_BTN_REGISTER);
            btn_delete.Create(Handle(), L"削除", 230, 10, ID_BTN_DELETE);

            label_category.Create(Handle(), L"分類", 10, 50, kLabelWidth, kLabelHeight);
            label_person.Create(Handle(), L"担当者", 140, 50, kLabelWidth, kLabelHeight);
            label_priority.Create(Handle(), L"優先度", 270, 50, kLabelWidth, kLabelHeight);
            label_status.Create(Handle(), L"ステータス", 400, 50, kLabelWidth, kLabelHeight);

            combo_category.Create(Handle(), 10, 70, kComboWidth, kComboHeight, ID_COMBO_CATEGORY);
            combo_person.Create(Handle(), 140, 70, kComboWidth, kComboHeight, ID_COMBO_PERSON);
            combo_priority.Create(Handle(), 270, 70, kComboWidth, kComboHeight, ID_COMBO_PRIORITY);
            combo_status.Create(Handle(), 400, 70, kComboWidth, kComboHeight, ID_COMBO_STATUS);

            task_list.Create(Handle(), 10, 110, 760, 400, ID_TASK_LIST);
            task_list.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

            const wchar_t* headers[COL_NUM] = {
                L"分類", L"タスク名", L"担当者", L"開始日", L"終了日", L"優先度", L"進捗率", L"ステータス"
            };
            const int widths[COL_NUM] = {280, 250, 250, 100, 100, 60, 100, 80};
            for (int i = 0; i < COL_NUM; ++i) {
                task_list.InsertColumn(i, headers[i], widths[i]);
            }

            std::wstring task_file_path = TaskRepository::ResolveTaskFilePath();
            if (!TaskRepository::LoadTasks(task_file_path, tasks_)) {
                std::wstring msg = L"task.txt の読み込みに失敗しました。\n読み込み先: " + task_file_path;
                MessageBoxW(Handle(), msg.c_str(), L"読み込みエラー", MB_OK | MB_ICONWARNING);
            }

            FillComboBoxWithAll(combo_category, GetUniqueCategories(tasks_));
            FillComboBoxWithAll(combo_person, GetUniquePersons(tasks_));
            FillComboBoxWithAll(combo_priority, GetUniquePriorities(tasks_));
            FillComboBoxWithAll(combo_status, GetUniqueStatuses(tasks_));
            ApplyFilter();
            SetupFont();
            return 0;
        }

        case WM_COMMAND: {
            int id = LOWORD(wp);
            int code = HIWORD(wp);

            if (code == CBN_SELCHANGE) {
                ApplyFilter();
                return 0;
            }

            if (id == ID_BTN_TO_LOGIN) {
                SendMessageW(GetParent(Handle()), MSG_GOTO_LOGIN, 0, 0);
                return 0;
            }
            if (id == ID_BTN_REGISTER) {
                MessageBoxW(Handle(), L"登録画面遷移は次ステップで実装予定です。", L"確認", MB_OK);
                return 0;
            }
            if (id == ID_BTN_DELETE) {
                MessageBoxW(Handle(), L"削除処理は次ステップで実装予定です。", L"確認", MB_OK);
                return 0;
            }
            return 0;
        }

        case WM_NOTIFY: {
            auto* pnmh = reinterpret_cast<LPNMHDR>(lp);
            if (HandleHeaderNotify(pnmh)) {
                return TRUE;
            }
            if (HandleItemActivate(pnmh)) {
                return 0;
            }
            return HandleCustomDraw(pnmh);
        }

        case WM_DESTROY:
            if (font_ != nullptr) {
                DeleteObject(font_);
                font_ = nullptr;
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}

void TaskViewer::onResize(int w, int h) {
    const int margin_top = 10;
    const int margin_left = 10;
    const int gap = 10;

    MoveWindow(btn_to_login.Handle(), margin_left, margin_top, kButtonWidth, kButtonHeight, TRUE);
    MoveWindow(btn_register.Handle(), margin_left + kButtonWidth + gap, margin_top, kButtonWidth, kButtonHeight, TRUE);
    MoveWindow(btn_delete.Handle(), margin_left + (kButtonWidth + gap) * 2, margin_top, kButtonWidth, kButtonHeight, TRUE);

    int label_top = margin_top + kButtonHeight + gap;
    MoveWindow(label_category.Handle(), margin_left, label_top, kLabelWidth, kLabelHeight, TRUE);
    MoveWindow(label_person.Handle(), 140, label_top, kLabelWidth, kLabelHeight, TRUE);
    MoveWindow(label_priority.Handle(), 270, label_top, kLabelWidth, kLabelHeight, TRUE);
    MoveWindow(label_status.Handle(), 400, label_top, kLabelWidth, kLabelHeight, TRUE);

    int combo_top = label_top + 20;
    MoveWindow(combo_category.Handle(), margin_left, combo_top, kComboWidth, kComboHeight, TRUE);
    MoveWindow(combo_person.Handle(), 140, combo_top, kComboWidth, kComboHeight, TRUE);
    MoveWindow(combo_priority.Handle(), 270, combo_top, kComboWidth, kComboHeight, TRUE);
    MoveWindow(combo_status.Handle(), 400, combo_top, kComboWidth, kComboHeight, TRUE);

    int list_top = combo_top + 25 + gap;
    MoveWindow(task_list.Handle(), margin_left, list_top, w - 20, h - list_top - 10, TRUE);
}