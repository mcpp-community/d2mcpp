<div align=right>

  [中文] | [繁體中文] | [English] | [Todo]
</div>

<div align=center>
  <img width="320" src="https://github.com/user-attachments/assets/dcc6bdea-71f8-4ed5-b445-6449c88655f3">

  <em>動手做 | 現代CPP核心語言特性 - <b>"強調動手實作的C++教學專案"</b></em>

  <b> [📚Book] + [🎥Video] + [⌨️Code] + [👥X] </b>
</div>

[中文]: README.zh.md
[繁體中文]: README.zh.hant.md
[English]: README.md
[Todo]: README.md

[📚Book]: https://mcpp-community.github.io/d2mcpp
[🎥Video]: https://youtube.com/playlist?list=PL7uow6t1QjF0ooMLkLSS96swpSuBZvoRE&si=1xHOGVIYpbzZAosI
[⌨️Code]: https://github.com/mcpp-community/d2mcpp/tree/main/src
[👥X]: https://forum.d2learn.org/category/20

## 目標

- **`[掌握]`** - **現代C++核心的語言特性**及使用場景
- **`[掌握]`** - 透過編譯器報錯資訊**定位問題的能力**
- **`[熟悉]`** - 透過文件和[cppreference](https://cppreference.com)解決C++中不熟悉問題的能力
- **`[了解]`** - 如何參與技術社群 - 開源專案的使用、提出問題、參與討論或貢獻

> [!CAUTION]
> 該項目當前處於開發初期, 如果你發現任何問題, 歡迎創建issues反饋或直接提交PR進行修復

## 快速開始

> 嘗試 `Code -> Book -> Video -> X -> Code`

### 線上程式碼練習

[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://github.com/codespaces/new?hide_repo_select=true&ref=main&repo=mcpp-community/d2mcpp)

等待環境準備完成（首次啟動會下載工具鏈並驗證參考答案），然後在終端執行 `d2x checker`，或使用 **Tasks: Run Task → d2mcpp: practice**。重新連線不會重複啟動 checker。準備失敗時查看[環境疑難排解](docs/environment.md)。

### 建立本地練習環境

支援的發行平台：Linux x86_64、macOS 14+ Apple Silicon、Windows x86_64（Windows PowerShell 5.1+）。mcpp 管理 C++ 工具鏈，Visual Studio 為選用項目。目前課程使用的 d2x Release 尚未涵蓋 Linux ARM64。可自行選擇編輯器，課程不會強制安裝。

**第一步：取得課程**（建議先 fork，用 git 儲存自己的練習進度）：

```bash
git clone https://github.com/mcpp-community/d2mcpp.git
cd d2mcpp
```

**第二步：準備環境。** 指令碼安裝 xlings **2026.10.18.1** 及 `.xlings.json` 宣告的版本（mcpp **2026.10.17.1**、d2x **2026.08.02.2**、mdBook **0.4.43**），建置並檢查 Provider。首次建置可能需要幾分鐘。

Linux / macOS：

```bash
bash scripts/setup.sh
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell：

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\setup.ps1
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

PATH 指令讓目前終端立即找到工具；安裝後重新開啟終端即可供後續工作階段使用。準備指令碼可重複執行，不會更動練習檔案或學習進度。

<details>
<summary>選用：透過 d2x install 下載課程</summary>

先安裝 [xlings](https://github.com/openxlings/xlings)：

```bash
curl -fsSL https://raw.githubusercontent.com/openxlings/xlings/v2026.10.18.1/tools/other/quick_install.sh | bash -s v2026.10.18.1
export PATH="$HOME/.xlings/subos/current/bin:$PATH"
```

Windows PowerShell 使用：

```powershell
irm https://raw.githubusercontent.com/mcpp-community/d2mcpp/main/scripts/install-xlings.ps1 | iex
$env:Path = "$env:USERPROFILE\.xlings\subos\current\bin;$env:Path"
```

然後在任一系統執行：

```text
xlings install d2x@2026.08.02.2 -y
d2x install d2mcpp
cd d2mcpp
```

繼續執行第二步中對應系統的準備指令碼；兩種下載方式共用下方驗證步驟。

</details>

**第三步：驗證環境並開始練習：**

```bash
mcpp test -p solutions       # 參考答案應全部通過（目前 52 份）
d2x checker                  # 編輯 -> 儲存 -> 自動偵測 -> 推進
d2x status                   # 進度總覽
```

未完成的練習報錯是預期行為，參考答案必須全綠。`Provider` 錯誤或連結路徑含 `/nonexistent/` 屬於環境問題，請查看[診斷與恢復步驟](docs/environment.md)。`git pull` 後重新執行準備指令碼，讓更新後的版本設定生效。

也可使用 `mcpp test -p src/cpp11` 直接練習某一章。更多操作見[使用說明](https://mcpp-community.github.io/d2mcpp/base/chapter_1.html)。


## 社區

- **即時交流:** 167535744、1067245099
- [**論壇版塊:**](https://forum.d2learn.org/category/20) 問題回饋、練習程式碼、技術交流與討論
- **社區活動:** [📣 MSCP - mcpp計畫學習與貢獻者培養計畫](https://moga.d2learn.org/activity/mscp/intro.html)

> **註:** 複雜性問題(技術、環境搭建等問題)推薦在論壇發文, 並詳細描述問題細節, 能更有效於問題的解決與復用

## 參與貢獻

- **參與社群交流:** 回饋問題、參與社群問題討論、幫助社群新用戶解決問題
- **參與專案維護與開發:** 參與社區中問題處理、修復Bug、多語言支援、[加入MSCP活動小組](https://moga.d2learn.org/activity/mscp/docs/join-group.html)、開發&最佳化新功能/模組

**📑開源協議與貢獻許可(License & CLA)**

- 本項目歡迎自由使用與分發！你可以在 [Apache License 2.0](LICENSE-CODE) 和 [CC-BY-NC-SA 4.0](LICENSE-BOOK) 協議下**免費**使用、修改和分享本專案的程式碼與文件內容
- 如希望參與貢獻程式碼或文檔，請先閱讀[貢獻者授權協議（CLA）](CLA.md)

**👥貢獻者**

[![Star History Chart](https://api.star-history.com/svg?repos=mcpp-community/d2mcpp&type=date&legend=top-left)](https://www.star-history.com/#mcpp-community/d2mcpp&type=date&legend=top-left)

<a href="https://github.com/mcpp-community/d2mcpp/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=mcpp-community/d2mcpp" />
</a>

<a href="https://hellogithub.com/en/repository/mcpp-community/d2mcpp" target="_blank"><img src="https://api.hellogithub.com/v1/widgets/recommend.svg?rid=7877f7cb12e940a5a432d49c19a360df&claim_uid=aNLTSv91Awj8ruX&theme=dark" alt="Featured｜HelloGitHub" style="width: 250px; height: 54px;" width="250" height="54" /></a>
