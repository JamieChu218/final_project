# 開發流程 (Development Process)

專題名稱：商店庫存管理系統　　學號：4B4G0039

本專題採 **逐代開發 (Iterative / Incremental Development)**，
分為五個開發代次 (iterations)，每一代都可獨立編譯執行，再逐步加入新功能。

---

## 開發代次 (Iterations)

### 第 1 代：建立繼承架構
- 設計抽象基底類別 `Product`（共用屬性與純虛擬函式）。
- 實作 `Food`、`Electronics`、`Clothing` 三個衍生類別。
- 驗證：能以 `Product*` 指標呼叫 `displayDetail()`，輸出各自格式（多型）。

### 第 2 代：庫存管理核心
- 建立 `Inventory` 類別，以 `std::vector<std::unique_ptr<Product>>` 儲存商品。
- 實作新增、查詢（依編號 / 依名稱）、刪除、進貨、銷售。
- 驗證：可正確新增與查詢商品，銷售不會造成負庫存。

### 第 3 代：終端機 UI
- 撰寫 `main.cpp` 文字選單與主迴圈。
- 加入 `readInt / readDouble / readLine` 輸入工具與錯誤處理。
- 驗證：所有選單項目皆可操作。

### 第 4 代：檔案讀寫
- 實作 `serialize()` 與 `saveToFile()` 寫檔。
- 實作 `loadFromFile()` 讀檔（依類別關鍵字建立對應物件）。
- 加入「啟動自動載入、離開自動存檔」。
- 驗證：關閉程式再開啟，資料仍存在。

### 第 5 代：統計報表與文件
- 以 `std::map` 實作 `displayReport()` 類別統計。
- 撰寫 README、規格書、開發流程與期末報告。
- 整理 Makefile，確保 `-Wall -Wextra` 無警告。

---

## 開發環境

| 項目 | 內容 |
|------|------|
| 作業系統 | Linux (Ubuntu 24.04) |
| 編譯器 | g++ 13.3 (C++14) |
| 建置工具 | GNU Make |
| 版本控制 | Git / GitHub |

---

## 編譯與測試紀錄

```bash
$ make
g++ -std=c++14 -Wall -Wextra -Isrc -c src/Product.cpp -o build/Product.o
...
g++ -std=c++14 -Wall -Wextra -Isrc -o inventory build/*.o
# 編譯成功，無任何 warning

$ ./inventory
已從 data/inventory.txt 載入 6 筆商品資料。
...
```

測試方式：以選單逐項操作（新增→顯示→銷售→報表→存檔→離開→重開驗證資料保留），
並針對「銷售數量大於庫存」「輸入非數字」「查無編號」等邊界條件確認系統正確反應。

---

## 未來可擴充方向

- 增加供應商、進貨單據與銷售歷史紀錄。
- 以 `std::map<int, ...>` 建立編號索引，加速大量資料查詢。
- 改用 CSV / JSON 格式，便於與試算表交換資料。
- 加入登入權限與操作日誌。
