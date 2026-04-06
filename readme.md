# MyWinAPI 統合版ガイド

このプロジェクトは、`src/MyWinAPI` の共通部品を使ってタスク管理アプリを構築する構成です。  
旧 `application` ディレクトリの実装は **すべて `src` に統合済み** です。

## ディレクトリ構成

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
ShareFolderPath.txt
```

## ビルド

```bash
g++ src/*.cpp src/MyWinAPI/*.cpp -mwindows -municode -lcomctl32 -o app.exe
```

- `-municode`: `wWinMain` を使うために必要
- `-lcomctl32`: `ListView` など共通コントロールを使うために必要

## 画面仕様（現状）

### ログイン画面 (`Login`)

- ユーザ名プルダウン
- パスワード入力欄
- `ログインする` ボタン
- `新規登録` ボタン（ログイン導線から離して右下配置）

> 以前の `タスク一覧へ` ボタンは削除済みです。

### タスク一覧画面 (`TaskViewer`)

- `ListView` による一覧表示
- 行チェックボックス
- フィルタ（分類 / 担当者 / 優先度 / ステータス）
- タスク名ダブルクリックで詳細表示
- 進捗率カラムのカスタム描画（バー + `%`）

## task.txt の読み込み先

`task.txt` は次のルールで決定します。

1. `ShareFolderPath.txt` の1行目を読む（共有フォルダのディレクトリパス）
2. `そのディレクトリ + "\\task.txt"` を読み込む
3. `ShareFolderPath.txt` が無い/空/不正なら、ルートの `task.txt` を使う

### `ShareFolderPath.txt` 例

```text
\\server\share\projectA
```

またはローカル確認用:

```text
.
```

`task.txt` の保存先も同じポリシーで扱う想定です（今後の保存処理追加時に同じパス解決を利用）。

## MyWinAPI の主な部品

- `Window`: Win32 ウィンドウ生成・`WindowProc` 連携
- `Screen`: 画面単位の `Show/Hide` と `onResize`
- `Control`: 共通コントロール基底
- `Button`, `Checkbox`, `EditBox`, `PullDown`, `Label`, `ListView`

### 追加/拡張された部品

- `Label`: ラベル表示用
- `ListView`: カラム追加、行追加、テキスト設定、拡張スタイル、ヘッダ取得
- `PullDown`: `ClearItems`, `SetSelectedIndex`, `GetTextByIndex`, `GetSelectedText`
- `EditBox`: `CreatePassword`（パスワード入力用）

## 新しい画面を作る手順

1. `src/YourScreen.h/.cpp` を作成（`Screen` 継承）
2. `HandleMessage` で `WM_CREATE`, `WM_COMMAND`, `WM_NOTIFY` を処理
3. `onResize` でレイアウトを再配置
4. `src/Message.h` に遷移メッセージを追加
5. `src/main.cpp` の親ウィンドウで画面生成と遷移切替を追加

### 画面クラス例（最小）

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

## 主要ユーティリティの責務

- `TaskRepository`
  - 共有パス解決 (`ResolveTaskFilePath`)
  - ファイル読み込み (`LoadTasks`)
- `TaskFilter`
  - フィルタ候補のユニーク値生成
- `TaskViewer`
  - UI表示、イベント処理、フィルタ反映

## 現在未実装の業務処理

- ログイン認証の本処理
- 新規登録画面/登録処理
- タスク登録・削除の本処理
- `task.txt` への保存処理

## トラブルシュート

- `task.txt` を読めない
  - `ShareFolderPath.txt` の1行目が正しいディレクトリか確認
  - そのディレクトリに `task.txt` が存在するか確認
- `ListView` が表示されない
  - `-lcomctl32` が付いているか確認
  - `ListView::InitCommonControls()` が呼ばれているか確認
- 日本語が崩れる
  - ファイルを UTF-8 で保存
  - 文字列は `L"..."` を使う