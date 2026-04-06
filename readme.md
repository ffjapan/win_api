# MyWinAPI + タスクアプリ統合ガイド

このリポジトリは、Win32 API を直接書く量を減らすための `MyWinAPI` と、  
タスク一覧アプリ（`application` の実装）を統合したプロジェクトです。

現在は `src/` 側で動作する構成に移行済みで、`application/` は移行元として残しています。  
最終的に `application/` を削除できるよう、機能を段階的に `src/` に集約します。

## 1. 現在の構成

```text
src/
  main.cpp
  Message.h
  Login.h / Login.cpp
  TaskViewer.h / TaskViewer.cpp
  TaskData.h
  TaskRepository.h / TaskRepository.cpp
  TaskFilter.h / TaskFilter.cpp
  MyWinAPI/
    Window.h / Window.cpp
    Screen.h / Screen.cpp
    Control.h / Control.cpp
    Button.h / Button.cpp
    Checkbox.h / Checkbox.cpp
    EditBox.h / EditBox.cpp
    PullDown.h / PullDown.cpp
    Label.h / Label.cpp
    ListView.h / ListView.cpp

task.txt
application/  (移行元、将来削除予定)
```

## 2. ビルド・実行

```bash
g++ src/*.cpp src/MyWinAPI/*.cpp -mwindows -municode -lcomctl32 -o app.exe
```

- `-municode`: `wWinMain` を使うために必要
- `-lcomctl32`: `ListView` など共通コントロールを使うために必要

## 3. 統合済み機能（`src/TaskViewer`）

- タスク一覧（`ListView`）
- 行チェックボックス
- 分類 / 担当者 / 優先度 / ステータスの絞り込み
- タスク名ダブルクリックで詳細ポップアップ表示
- 進捗率カラムのカスタム描画（バー + `%` 表示）
- `task.txt` からの読み込み（UTF-8テキスト）

> なお、登録ボタン・削除ボタンの業務処理は次段階で実装する想定です。

## 4. MyWinAPI の設計方針

### Window 層

- `Window`
  - `CreateWindowExW` のラップ
  - `WindowProc` と C++ オブジェクトの接続

### Screen 層

- `Screen`
  - `Show()` / `Hide()`
  - `onResize(w, h)` フック
  - 共通背景処理

### Control 層

- `Control`
  - コントロール生成の共通基底
  - `HWND` の保持
- 個別部品
  - `Button`, `Checkbox`, `EditBox`, `PullDown`, `Label`, `ListView`

## 5. 新しい画面クラスの作り方

### 5.1 ヘッダ作成（`src/SampleScreen.h`）

```cpp
#ifndef APP_SAMPLESCREEN_H
#define APP_SAMPLESCREEN_H

#include "MyWinAPI/Screen.h"
#include "MyWinAPI/Button.h"

class SampleScreen : public Screen {
public:
    Button btn_back;
protected:
    LRESULT HandleMessage(UINT msg, WPARAM wp, LPARAM lp) override;
    void onResize(int w, int h) override;
};

#endif
```

### 5.2 実装作成（`src/SampleScreen.cpp`）

```cpp
#include "SampleScreen.h"
#include "Message.h"

enum {
    ID_BTN_BACK = 9001
};

LRESULT SampleScreen::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            btn_back.Create(Handle(), L"戻る", 10, 10, ID_BTN_BACK);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == ID_BTN_BACK) {
                SendMessageW(GetParent(Handle()), MSG_GOTO_LOGIN, 0, 0);
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}

void SampleScreen::onResize(int w, int h) {
    MoveWindow(btn_back.Handle(), 10, 10, 120, 30, TRUE);
}
```

### 5.3 `main.cpp` に画面を登録

1. `#include "SampleScreen.h"` を追加
2. 親ウィンドウクラスに `SampleScreen screen_sample;` を追加
3. `WM_CREATE` で `screen_sample.Create(...)`
4. `Message.h` に必要な遷移メッセージを追加
5. 親の `HandleMessage` で `Show()/Hide()` を切り替え

## 6. 主要コンポーネント利用例

### PullDown（新メソッド含む）

```cpp
PullDown combo;
combo.Create(Handle(), 10, 10, 180, 300, 1001);
combo.ClearItems();
combo.AddItem(L"ALL");
combo.AddItem(L"未着手");
combo.SetSelectedIndex(0);
std::wstring selected = combo.GetSelectedText();
```

### ListView

```cpp
ListView::InitCommonControls();

ListView list;
list.Create(Handle(), 10, 40, 600, 300, 2001);
list.SetExtendedStyle(LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
list.InsertColumn(0, L"タスク名", 240);
list.InsertColumn(1, L"進捗率", 100);
list.InsertItem(0, L"要件整理");
list.SetItemText(0, 1, L"80");
```

## 7. データ読み込みの役割分離

- `TaskRepository`
  - `task.txt` の読み込みと `TaskData` への変換
- `TaskFilter`
  - フィルタ用ユニーク値生成（分類/担当者/優先度/ステータス）
- `TaskViewer`
  - UI描画・イベント処理・フィルタ適用

この分離により、将来 `TaskCreateScreen` を追加しても  
データ処理を再利用しやすい構成になっています。

## 8. `application/` からの移行ポリシー

- `application` は **参照元** として扱う
- 新規実装はすべて `src/` 側に置く
- `application` 側にしかないロジックがゼロになった時点で削除する

## 9. 今後の拡張予定（推奨）

1. 登録画面 (`TaskCreateScreen`) を `Screen` として追加
2. 作成処理（CSV追記）を `TaskRepository` に追加
3. 登録完了ポップアップ後に一覧再読込
4. 削除ボタンの実処理
5. レイアウトヘルパ（座標直書きの削減）

## 10. トラブルシュート

- `ListView` が表示されない
  - `ListView::InitCommonControls()` 呼び出しを確認
  - ビルドオプションに `-lcomctl32` があるか確認
- 文字化けする
  - `L"..."` のワイド文字列を使う
  - UTF-8 で `task.txt` を保存する
- コンボ変更で絞り込まれない
  - `WM_COMMAND` で `CBN_SELCHANGE` を処理しているか確認