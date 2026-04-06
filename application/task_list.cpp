#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <sstream>

#include <iostream>

#include "read_file.h"
#include "filter.h"

//ボタン番号
#define ID_REGISTER 1	//登録
#define ID_DELETE 2	//削除

//ボタンサイズ・位置
#define BTN_WSIZE 	100	//横
#define BTN_HSIZE 	25	//縦
#define BTN_Y		   		10	//位置（高さ）
#define BTN_X_REGISTER	10	//位置（横）
#define BTN_X_DELETE		120	//位置（横）

//プルダウンサイズ・位置
#define CMB_WSIZE 120	//横
#define CMB_HSIZE 300	//縦　※小さくするとプルダウンの選択肢消える
#define CMB_Y			 	40	//位置（高さ）
#define CMB_X_CATEGORY	10	//位置（横）
#define CMB_X_PERSON		140	//位置（横）
#define CMB_X_PRIORITY	270	//位置（横）
#define CMB_X_STATUS		400	//位置（横）

//ラベルサイズ・位置
#define LBL_WSIZE 120	//横
#define LBL_HSIZE 20	//縦
#define LBL_Y			    20	//位置（高さ）
#define LBL_X_CATEGORY	10	//位置（横）
#define LBL_X_PERSON		140	//位置（横）
#define LBL_X_PRIORITY	270	//位置（横）
#define LBL_X_STATUS		400	//位置（横）

//フォントサイズ
#define FONT_SIZE -14	// 高さ（負 = 論理単位）

//リストビューサイズ・位置
#define	LST_WSIZE	760	//横
#define	LST_HSIZE	400	//縦
#define	LST_Y		10	//位置（高さ）
#define	LST_X		80	//位置（横）

//リスト幅
#define COL_SIZE_CATEGORY 280		//分類
#define COL_SIZE_TASK_PERSON 250	//タスク名と担当者
#define COL_SIZE_DATE 100			//開始日・終了日
#define COL_SIZE_PRIORITY 60		//優先度
#define COL_SIZE_PROGRESS 100		//進捗率
#define COL_SIZE_STATUS 80		//ステータス

//リスト列番号
#define COL_NUM		8	//列数
#define COL_CATEGORY 	0	//分類
#define COL_TASKNAME 	1	//タスク名
#define COL_PERSON 	2	//担当者
#define COL_START 		3	//開始日
#define COL_END 		4	//終了日
#define COL_PRIORITY 	5	//優先度
#define COL_PROGRESS 	6	//進捗率
#define COL_STATUS 	7	//ステータス


//==============================================
// --- よく使うやつ---
//==============================================
struct UIControls {
    HWND h_list;				//リスト
    HWND h_button_register;		//登録ボタン
    HWND h_button_delete;		//削除ボタン
    HWND h_combo_category;		//分類プルダウン
    HWND h_combo_person;		//担当者プルダウン
    HWND h_combo_priority;		//優先度プルダウン
    HWND h_combo_status;		//ステータスプルダウン
    HWND h_label_category;		//分類ラベル
    HWND h_label_person;		//担当者ラベル
    HWND h_label_priority;		//優先度ラベル
    HWND h_label_status;		//ステータスラベル
    HFONT h_font;				//フォント
};

//==============================================
// --- フォント設定 ---
//==============================================
void setFontAll(HFONT font, std::initializer_list<HWND> controls) {
    for (auto h : controls) {
        SendMessageW(h, WM_SETFONT, (WPARAM)font, TRUE);
    }
}

//==============================================
// --- プルダウンで選択されている文字列を取得---
//==============================================
std::wstring getComboBoxText(HWND hCombo) {
    LRESULT sel = SendMessageW(hCombo, CB_GETCURSEL, 0, 0);	//今選ばれてる項目の番号

    //---失敗---
    if (sel == CB_ERR){
    	return L"";
    }

    LRESULT len = SendMessageW(hCombo, CB_GETLBTEXTLEN, sel, 0);//選択項目の文字数

    //---失敗---
    if (len == CB_ERR){
    	return L"";
    }

    std::wstring buf(len + 1, L'\0');

    //文字列取得
    SendMessageW(hCombo, CB_GETLBTEXT, sel, (LPARAM)buf.data());
    buf.resize(len);//サイズ調整
    return buf;
}

//==============================================
// --- リスト設定---
//==============================================
bool handleHeaderNotify(UIControls& ui, LPNMHDR pnmh) {
    HWND hHeader = ListView_GetHeader(ui.h_list);

    // ボタン風削除（毎回やる必要ないけど一旦ここ）
    LONG style = GetWindowLong(hHeader, GWL_STYLE);
    style &= ~HDS_BUTTONS;
    SetWindowLong(hHeader, GWL_STYLE, style);

    if (pnmh->hwndFrom == hHeader) {
        switch (pnmh->code) {
        case HDN_BEGINTRACKA:
        case HDN_BEGINTRACKW:
        case HDN_ITEMCHANGINGA:
        case HDN_ITEMCHANGINGW:
            return true; // 横幅変更禁止
        }
    }
    return false;
}

//==============================================
// --- ダブルクリック処理---
//==============================================
bool handleItemActivate(HWND hwnd, LPNMHDR pnmh, std::vector<taskData>& tasks_filtered) {
    if (pnmh->hwndFrom != NULL && pnmh->code == NM_DBLCLK) {

        LPNMITEMACTIVATE pItem = (LPNMITEMACTIVATE)pnmh;

        if (pItem->iItem >= 0 && pItem->iSubItem == COL_TASKNAME) {
            int row = pItem->iItem;
            taskData& t = tasks_filtered[row];

            std::wstring msg =
                L"タスク名: " + t.task_name + L"\n" +
                L"分類: " + t.category + L"\n" +
                L"担当者: " + t.person + L"\n" +
                L"開始日: " + t.start + L"\n" +
                L"終了日: " + t.end + L"\n" +
                L"優先度: " + t.priority + L"\n" +
                L"進捗: " + std::to_wstring(t.progress) + L"%\n" +
                L"ステータス: " + t.status;

            MessageBoxW(hwnd, msg.c_str(), L"タスク詳細", MB_OK);
            return true;
        }
    }
    return false;
}
//==============================================
// --- 進捗バー中身---
//==============================================
void drawProgressBar(HDC hdc, const RECT& rcCell, int progress){

	// まず背景を塗る
	FillRect(hdc, &rcCell, (HBRUSH)(COLOR_WINDOW+1));

	int padding = 2;
	int textWidth = 40;

	// バー領域
	RECT rcBar = rcCell;
	rcBar.left  += padding;
	rcBar.top   += padding;
	rcBar.bottom-= padding;
	rcBar.right -= textWidth;

	// 背景バー
	HBRUSH hBg = CreateSolidBrush(RGB(220,220,220));
	FillRect(hdc, &rcBar, hBg);
	DeleteObject(hBg);

	// 進捗
	int barWidth = rcBar.right - rcBar.left;
	int progressWidth = progress * barWidth / 100;

	RECT rcDone = rcBar;
	rcDone.right = rcDone.left + progressWidth;

	HBRUSH hDone = CreateSolidBrush(RGB(255,140,0));	//オレンジRGB(255,140,0)
	FillRect(hdc, &rcDone, hDone);
	DeleteObject(hDone);

	// テキスト
	std::wstring text = std::to_wstring(progress) + L"%";

	RECT rcText = rcCell;
	rcText.left = rcBar.right;

	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, text.c_str(), -1, &rcText,
			 DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

}

//==============================================
// --- 進捗バー---
//==============================================
LRESULT handleCustomDraw(UIControls& ui, LPNMHDR pnmh, std::vector<taskData>& tasks_filtered) {
    if (pnmh->hwndFrom != ui.h_list || pnmh->code != NM_CUSTOMDRAW)
        return CDRF_DODEFAULT;

    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pnmh;

    switch (lplvcd->nmcd.dwDrawStage) {
    case CDDS_PREPAINT:
        return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT:
        return CDRF_NOTIFYSUBITEMDRAW;

    case CDDS_SUBITEM | CDDS_ITEMPREPAINT: {
        int row = (int)lplvcd->nmcd.dwItemSpec;
        int col = lplvcd->iSubItem;

        if (row >= (int)tasks_filtered.size())
            return CDRF_DODEFAULT;

        // テキスト色リセット
        lplvcd->clrText = RGB(0, 0, 0);

        // リンク風
        if (col == COL_TASKNAME) {
            lplvcd->clrText = RGB(0, 102, 204);
        }

        // 進捗バー
        if (col == COL_PROGRESS) {
            RECT rcCell;
            ListView_GetSubItemRect(ui.h_list, row, col, LVIR_BOUNDS, &rcCell);

            taskData& t = tasks_filtered[row];

            drawProgressBar(lplvcd->nmcd.hdc, rcCell, t.progress);

            return CDRF_SKIPDEFAULT;
        }

        return CDRF_DODEFAULT;
    }
    }

    return CDRF_DODEFAULT;
}


//==============================================
// --- ウィンドウプロシージャ ---
//==============================================
LRESULT CALLBACK taskListProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	static UIControls ui;
    static std::vector<taskData> tasks;       // 全タスク
    static std::vector<taskData> tasks_filtered; // フィルター後

    //フィルタリング
    auto applyFilter = [&]() {
        tasks_filtered.clear();
        std::wstring fc = getComboBoxText(ui.h_combo_category);
        std::wstring fp = getComboBoxText(ui.h_combo_person);
        std::wstring fpr = getComboBoxText(ui.h_combo_priority);
        std::wstring fs = getComboBoxText(ui.h_combo_status);

        for (auto& t : tasks) {
            if ((fc != L"ALL" && t.category != fc) ||
                (fp != L"ALL" && t.person != fp) ||
                (fpr != L"ALL" && t.priority != fpr) ||
                (fs != L"ALL" && t.status != fs)) {
                continue;
            }
            tasks_filtered.push_back(t);
        }

        ListView_DeleteAllItems(ui.h_list);
        int row = 0;
        for (auto& t : tasks_filtered) {
            LVITEM item = {};
            item.mask = LVIF_TEXT;
            item.iItem = row;
            item.pszText = const_cast<LPWSTR>(t.task_name.c_str());
            ListView_InsertItem(ui.h_list, &item);

            ListView_SetItemText(ui.h_list, row, COL_CATEGORY, const_cast<LPWSTR>(t.category.c_str()));
            ListView_SetItemText(ui.h_list, row, COL_TASKNAME, const_cast<LPWSTR>(t.task_name.c_str()));
            ListView_SetItemText(ui.h_list, row, COL_PERSON, const_cast<LPWSTR>(t.person.c_str()));
            ListView_SetItemText(ui.h_list, row, COL_START, const_cast<LPWSTR>(t.start.c_str()));
            ListView_SetItemText(ui.h_list, row, COL_END, const_cast<LPWSTR>(t.end.c_str()));
            ListView_SetItemText(ui.h_list, row, COL_PRIORITY, const_cast<LPWSTR>(t.priority.c_str()));
            ListView_SetItemText(ui.h_list, row, COL_PROGRESS, const_cast<LPWSTR>(std::to_wstring(t.progress).c_str()));
            ListView_SetItemText(ui.h_list, row, COL_STATUS, const_cast<LPWSTR>(t.status.c_str()));
            row++;
        }
    };

    switch (msg) {
    case WM_CREATE: {
        INITCOMMONCONTROLSEX icex = { sizeof(icex), ICC_LISTVIEW_CLASSES };
        InitCommonControlsEx(&icex);

        // ボタン
        ui.h_button_register = CreateWindowW(L"BUTTON", L"登録", WS_VISIBLE | WS_CHILD,
        									BTN_X_REGISTER, BTN_Y, BTN_WSIZE, BTN_HSIZE, hwnd,
											(HMENU)ID_REGISTER, NULL, NULL);
        ui.h_button_delete   = CreateWindowW(L"BUTTON", L"削除", WS_VISIBLE | WS_CHILD,
        									BTN_X_DELETE, BTN_Y, BTN_WSIZE, BTN_HSIZE, hwnd,
											(HMENU)ID_DELETE, NULL, NULL);

        // プルダウン
        ui.h_combo_category = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        									CMB_X_CATEGORY, CMB_Y, CMB_WSIZE, CMB_HSIZE, hwnd, NULL, NULL, NULL);
        ui.h_combo_person   = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        									CMB_X_PERSON, CMB_Y, CMB_WSIZE, CMB_HSIZE, hwnd, NULL, NULL, NULL);
        ui.h_combo_priority = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        									CMB_X_PRIORITY, CMB_Y, CMB_WSIZE, CMB_HSIZE, hwnd, NULL, NULL, NULL);
        ui.h_combo_status   = CreateWindowW(L"COMBOBOX", L"", WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST,
        									CMB_X_STATUS, CMB_Y, CMB_WSIZE, CMB_HSIZE, hwnd, NULL, NULL, NULL);
        //ラベル
        ui.h_label_category = CreateWindowW(L"STATIC", L"分類", WS_VISIBLE | WS_CHILD | SS_CENTER,
        									LBL_X_CATEGORY, LBL_Y, LBL_WSIZE, LBL_HSIZE, hwnd, NULL, NULL, NULL);
        ui.h_label_person   = CreateWindowW(L"STATIC", L"担当者", WS_VISIBLE | WS_CHILD | SS_CENTER,
        									LBL_X_PERSON, LBL_Y, LBL_WSIZE, LBL_HSIZE, hwnd, NULL, NULL, NULL);
        ui.h_label_priority = CreateWindowW(L"STATIC", L"優先度", WS_VISIBLE | WS_CHILD | SS_CENTER,
        									LBL_X_PRIORITY, LBL_Y, LBL_WSIZE, LBL_HSIZE, hwnd, NULL, NULL, NULL);
        ui.h_label_status   = CreateWindowW(L"STATIC", L"ステータス", WS_VISIBLE | WS_CHILD | SS_CENTER,
        									LBL_X_STATUS, LBL_Y, LBL_WSIZE, LBL_HSIZE, hwnd, NULL, NULL, NULL);

        //フォント
        ui.h_font = CreateFontW(
            FONT_SIZE,
            0, 0, 0,
            FW_NORMAL,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            L"Meiryo UI"
        );

        // データ読み込み
        loadTasks(L"task.txt", tasks);
        //コンボボックスに設定
        setComboItems(ui.h_combo_category, getUniqueValues(tasks, &taskData::category));
        setComboItems(ui.h_combo_person, getUniqueValues(tasks, &taskData::person));
        setComboItems(ui.h_combo_priority, getUniqueValues(tasks, &taskData::priority));
        setComboItems(ui.h_combo_status, getUniqueValues(tasks, &taskData::status));


        // リストビュー
        ui.h_list = CreateWindowW(WC_LISTVIEWW, L"", WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_SHOWSELALWAYS,
        						LST_X, LST_Y, LST_WSIZE, LST_HSIZE, hwnd, NULL, NULL, NULL);
        ListView_SetExtendedListViewStyle(ui.h_list, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

        // カラム
        LVCOLUMN col = {};
        col.mask = LVCF_TEXT | LVCF_WIDTH;
        const wchar_t* headers[COL_NUM] = { L"分類", L"タスク名", L"担当者", L"開始日", L"終了日", L"優先度", L"進捗率", L"ステータス" };
        int widths[COL_NUM] = { COL_SIZE_CATEGORY, COL_SIZE_TASK_PERSON, COL_SIZE_TASK_PERSON, COL_SIZE_DATE, COL_SIZE_DATE, COL_SIZE_PRIORITY, COL_SIZE_PROGRESS, COL_SIZE_STATUS };
        for (int i = 0; i < COL_NUM; i++) {
            col.cx = widths[i];
            col.pszText = (LPWSTR)headers[i];
            ListView_InsertColumn(ui.h_list, i, &col);
        }

        applyFilter();

        setFontAll(ui.h_font, {
            ui.h_list,
            ui.h_combo_category,
            ui.h_combo_person,
            ui.h_combo_priority,
            ui.h_combo_status,
            ui.h_button_register,
            ui.h_button_delete,
            ui.h_label_category,
            ui.h_label_person,
            ui.h_label_priority,
            ui.h_label_status
        });
        return 0;
    }

    case WM_NOTIFY: {
        LPNMHDR pnmh = (LPNMHDR)lp;

        if (handleHeaderNotify(ui, pnmh)) return TRUE;
        if (handleItemActivate(hwnd, pnmh, tasks_filtered)) return 0;

        return handleCustomDraw(ui, pnmh, tasks_filtered);
    }

    case WM_COMMAND: {
        int id = LOWORD(wp);
        int code = HIWORD(wp);

        // コンボボックス変更時だけ
        if (code == CBN_SELCHANGE) {
            applyFilter();
        }

        if (id == ID_REGISTER) {
            MessageBoxW(hwnd, L"登録ボタン押された", L"確認", MB_OK);
        }
        else if (id == ID_DELETE) {
            MessageBoxW(hwnd, L"削除ボタン押された", L"確認", MB_OK);
        }

        return 0;
    }

    case WM_SIZE: {
        int w = LOWORD(lp), h = HIWORD(lp);
        int marginTop = 10, marginLeft = 10, gap = 10;
        MoveWindow(ui.h_button_register, marginLeft, marginTop, BTN_WSIZE, BTN_HSIZE, TRUE);
        MoveWindow(ui.h_button_delete, marginLeft + BTN_WSIZE + gap, marginTop, BTN_WSIZE, BTN_HSIZE, TRUE);
        int labelTop = marginTop + BTN_HSIZE + gap;
        MoveWindow(ui.h_label_category, marginLeft, labelTop, LBL_WSIZE, LBL_HSIZE, TRUE);
        MoveWindow(ui.h_label_person, LBL_X_PERSON, labelTop, LBL_WSIZE, LBL_HSIZE, TRUE);
        MoveWindow(ui.h_label_priority, LBL_X_PRIORITY, labelTop, LBL_WSIZE, LBL_HSIZE, TRUE);
        MoveWindow(ui.h_label_status, LBL_X_STATUS, labelTop, LBL_WSIZE, LBL_HSIZE, TRUE);
        int comboTop = labelTop + 20;
        MoveWindow(ui.h_combo_category, marginLeft, comboTop, CMB_WSIZE, CMB_HSIZE, TRUE);
        MoveWindow(ui.h_combo_person, CMB_X_PERSON, comboTop, CMB_WSIZE, CMB_HSIZE, TRUE);
        MoveWindow(ui.h_combo_priority, CMB_X_PRIORITY, comboTop, CMB_WSIZE, CMB_HSIZE, TRUE);
        MoveWindow(ui.h_combo_status, CMB_X_STATUS, comboTop, CMB_WSIZE, CMB_HSIZE, TRUE);
        int listTop = comboTop + 25 + gap;
        MoveWindow(ui.h_list, 10, listTop, w - 20, h - listTop - 10, TRUE);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

//==============================================
// --- ウィンドウ作成 ---
//==============================================
void createTaskList(HINSTANCE hInst) {

	std::cout << "a" <<std::endl;
    WNDCLASSW wc = {};
    wc.lpfnWndProc = taskListProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"task_list";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc)) {
        MessageBox(NULL, L"RegisterClass失敗", L"", MB_OK);
        return;
    }

    HWND hwnd = CreateWindowW(
        L"task_list", L"タスク一覧",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInst, NULL
    );

    if (!hwnd) {
        MessageBox(NULL, L"CreateWindow失敗", L"", MB_OK);
        return;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

