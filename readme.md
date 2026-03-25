# MyWinAPI 利用ガイド（詳細版）

このプロジェクトは、Win32 API を直接書く量を減らすために、`src/MyWinAPI` に最小限のラッパーを置いた構成です。  
アプリ固有の画面（`Login`、`TaskViewer` など）は `src/` 直下に置き、共通UI部品は `src/MyWinAPI/` にまとめます。

---

## 1. ディレクトリ構成

```text
src/
  main.cpp               // アプリ起動・親ウィンドウ・画面切替
  Message.h              // 画面間通信用の WM_APP メッセージ定義
  Login.h / Login.cpp    // 画面クラス（アプリ固有）
  TaskViewer.h / TaskViewer.cpp
  MyWinAPI/
    Window.h / Window.cpp
    Screen.h / Screen.cpp
    Control.h / Control.cpp
    Button.h / Button.cpp
    Checkbox.h / Checkbox.cpp
    EditBox.h / EditBox.cpp
    PullDown.h / PullDown.cpp
```

---

## 2. ビルド方法

### 2.1 最短ビルド

```bash
g++ src/*.cpp src/MyWinAPI/*.cpp -mwindows -municode
```

### 2.2 出力ファイル名を指定する場合

```bash
g++ src/*.cpp src/MyWinAPI/*.cpp -mwindows -municode -o app.exe
```

> `-municode` を付けることで `wWinMain`（ワイド文字エントリポイント）を使用できます。

---

## 3. MyWinAPI の基本思想

MyWinAPI は次の3層で構成されています。

1. **Window層**  
   生の `HWND` を管理し、`WindowProc` と C++ クラスを接続します。
2. **Screen層**  
   画面（ページ）として表示/非表示やリサイズフックを提供します。
3. **Control層**  
   ボタン・チェックボックス・入力欄などの共通生成処理を持ちます。

---

## 4. クラスごとの役割

## `Window`

- ファイル: `src/MyWinAPI/Window.h`, `src/MyWinAPI/Window.cpp`
- 役割:
  - `CreateWindowExW` を実行
  - `WindowProc` で `this` ポインタを紐付け
  - `virtual HandleMessage(...)` で派生先へイベントを委譲

### 主なメソッド

- `bool Create(...)`
  - ウィンドウ生成
- `HWND Handle() const`
  - 内部 `HWND` 取得
- `static LRESULT CALLBACK WindowProc(...)`
  - Win32 コールバック入口

---

## `Screen`

- ファイル: `src/MyWinAPI/Screen.h`, `src/MyWinAPI/Screen.cpp`
- 役割:
  - 画面単位で `Show()` / `Hide()` を提供
  - `onResize(int w, int h)` を仮想メソッドとして提供
  - 背景塗りつぶし（`WM_ERASEBKGND`）の共通化

### 派生時に主に触る点

- `LRESULT HandleMessage(...) override`
- `void onResize(int w, int h) override`

---

## `Control`

- ファイル: `src/MyWinAPI/Control.h`, `src/MyWinAPI/Control.cpp`
- 役割:
  - 各コントロールの共通生成関数 `CreateControl(...)` を提供
  - `HWND` の保持

---

## 個別コントロール

- `Button`
  - 押しボタン生成（`BS_PUSHBUTTON`）
- `Checkbox`
  - チェックボックス生成（`BS_CHECKBOX`）
  - `IsChecked()` で状態取得
- `EditBox`
  - テキスト入力欄生成
  - `GetText()` で文字列取得
- `PullDown`
  - プルダウン（コンボボックス）生成
  - `AddItem()` / `GetSelectedIndex()`

---

## 5. 画面遷移の仕組み（現行）

画面遷移は `WM_APP + n` のカスタムメッセージで行います。

- 定義場所: `src/Message.h`
- 例:
  - `MSG_GOTO_LOGIN`
  - `MSG_GOTO_TASKVIEWER`

`main.cpp` 内の親ウィンドウ（`MyWindow`）がこのメッセージを受け取り、`screen_login.Show()` / `Hide()` などを切り替えます。

---

## 6. 新しい画面クラスの作り方（手順）

ここでは `SampleScreen` を追加する例を示します。

### 手順1: ヘッダ作成（`src/SampleScreen.h`）

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

#endif  // APP_SAMPLESCREEN_H
```

### 手順2: 実装作成（`src/SampleScreen.cpp`）

```cpp
#include "SampleScreen.h"
#include "Message.h"
#include <windows.h>

LRESULT SampleScreen::HandleMessage(UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE:
            btn_back.Create(Handle(), L"戻る", 20, 20, 201);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wp) == 201) {
                SendMessageW(GetParent(Handle()), MSG_GOTO_LOGIN, 0, 0);
            }
            return 0;
    }
    return Screen::HandleMessage(msg, wp, lp);
}

void SampleScreen::onResize(int w, int h) {
    MoveWindow(btn_back.Handle(), 20, 20, 120, 30, TRUE);
}
```

### 手順3: `main.cpp` に画面を登録

1. `#include "SampleScreen.h"` を追加
2. 親ウィンドウクラスにメンバを追加  
   - `SampleScreen screen_sample;`
3. `WM_CREATE` で `screen_sample.Create(...)`
4. 必要な `MSG_GOTO_...` を `src/Message.h` に追加
5. 親ウィンドウの `HandleMessage` で表示切替を追加

---

## 7. ボタン・入力欄など部品の使い方

## ボタン

```cpp
Button btn_save;
btn_save.Create(Handle(), L"保存", 10, 10, 1001);
```

- 押下判定は `WM_COMMAND` で `LOWORD(wp) == 1001`

## チェックボックス

```cpp
Checkbox chk_done;
chk_done.Create(Handle(), L"完了", 10, 50, 1002);
bool checked = chk_done.IsChecked();
```

## 入力欄

```cpp
EditBox edit_name;
edit_name.Create(Handle(), 10, 90, 240, 28, 1003);
std::wstring name = edit_name.GetText();
```

## プルダウン

```cpp
PullDown pd_status;
pd_status.Create(Handle(), 10, 130, 180, 300, 1004);
pd_status.AddItem(L"未着手");
pd_status.AddItem(L"進行中");
pd_status.AddItem(L"完了");
int selected = pd_status.GetSelectedIndex();
```

---

## 8. 実装ルール（推奨）

- 画面クラスは `src/` 直下（`XXX.h/.cpp`）
- 共通UI部品は `src/MyWinAPI/`
- 画面遷移は `src/Message.h` にまとめる
- 画面ごとにコントロールID重複を避ける
- ヘッダは必ずインクルードガードを付ける

---

## 9. よくあるハマりどころ

- **画面が表示されない**
  - `Create()` の親 `HWND` が正しいか確認
  - `WS_CHILD | WS_VISIBLE` が必要か確認
- **ボタンが反応しない**
  - `WM_COMMAND` の `LOWORD(wp)` がIDと一致しているか確認
- **リサイズでレイアウト崩れ**
  - `onResize()` で `MoveWindow()` を再配置しているか確認
- **文字化け**
  - `L"..."` のワイド文字列を使っているか確認
  - ビルド時に `-municode` が付いているか確認

---

## 10. 現状の拡張性で、MyWinAPIとして修正を検討すべき点（コード修正は未実施）

以下は「今後の拡張に備えるための改善提案」です。  
このREADME更新では、実際のプログラム修正は行っていません。

1. **レイアウト管理がない（座標直書き前提）**
   - 現在は `MoveWindow(...)` の手動配置中心
   - 将来的に `Layout` クラス（縦積み・横並び・余白・比率）を導入すると画面追加が楽になる

2. **Control生成の引数が低レベル**
   - `CreateControl(...)` が Win32 引数そのままのため、利用者が詳細を毎回意識する必要がある
   - `ControlStyle` 構造体などで高レベル化すると使いやすい

3. **イベント処理が WM_COMMAND 依存で分散**
   - 画面ごとにID比較を手書きしており、画面が増えるほど保守負担が上がる
   - `OnClick(id, handler)` のようなディスパッチ層があると拡張しやすい

4. **コントロールID管理の仕組みがない**
   - 重複IDをコンパイル時に検知できない
   - 画面単位でID範囲を固定するか、ID発番ヘルパを導入したほうが安全

5. **`EditBox::GetText()` のバッファ境界が不安定**
   - 長さと終端文字の扱いを厳密にしたユーティリティ化が望ましい
   - 共通の文字列取得ヘルパに切り出すと再利用性が上がる

6. **アプリメッセージ定義が単純列挙のみ**
   - 画面数増加時に管理が煩雑化しやすい
   - 命名規約・採番規約・メッセージルータを整備すると拡張が楽になる

7. **複合部品（例: ラベル付き入力、日付入力）が未整備**
   - 業務画面では同じUI塊が繰り返される
   - 複合コントロール化で画面構築速度と一貫性が向上する

8. **データバインディング層がない**
   - コントロール値 ↔ モデルの同期を都度手書きする必要がある
   - `BindText`, `BindChecked` のような薄い仕組みがあると保守性が高い

9. **検証（バリデーション）共通化がない**
   - 入力チェックが画面ごとにバラバラになりやすい
   - エラー表示規約と検証ヘルパを持つと大型化に耐えやすい

10. **描画・見た目（フォント、色、余白）のテーマ管理がない**
    - 画面増加時に統一感を崩しやすい
    - テーマ設定（フォント/色/サイズ）を一元化すると変更に強い