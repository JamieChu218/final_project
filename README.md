# 商店庫存管理系統 (Store Inventory Management System)

> C++ 期末專題 ‧ 學號：4B4G0039

一套以 **C++ 物件導向** 撰寫、在 **終端機 (Terminal)** 上操作的商店庫存管理系統。
使用者可以透過文字選單新增 / 查詢 / 進貨 / 銷售 / 刪除商品，並將資料儲存為檔案，
下次開啟時自動載入。

---

## 一、專題符合的六大要求

| # | 題目要求 | 本專題的實作方式 |
|---|----------|------------------|
| 1 | **呈現 C++ 的類別繼承功能** | 抽象基底類別 `Product`，衍生出 `Food`、`Electronics`、`Clothing`，並以虛擬函式達成多型 |
| 2 | **讀檔與寫檔功能** | `Inventory::saveToFile()` 寫檔、`loadFromFile()` 讀檔；程式啟動自動載入、離開自動存檔 |
| 3 | **使用 STL 類別庫** | `std::vector`、`std::map`、`std::unique_ptr`、`std::string`、`<algorithm>`、`<sstream>` |
| 4 | **使用終端機模擬完整的 UI 操作介面** | `main.cpp` 提供 12 項功能的文字選單與輸入錯誤處理 |
| 5 | **完整的規格說明與開發流程** | 見 [`docs/SPECIFICATION.md`](docs/SPECIFICATION.md) 與 [`docs/DEVELOPMENT.md`](docs/DEVELOPMENT.md) |
| 6 | **完整的說明文件** | 本 README、規格書、開發流程與期末報告 PDF |

---

## 二、檔案結構

```
final_project/
├── README.md                 專題總說明
├── Makefile                  一鍵編譯
├── data/
│   └── inventory.txt         範例 / 永久儲存的庫存資料
├── src/
│   ├── Product.h / .cpp      抽象基底類別
│   ├── Food.h / .cpp         食品 (衍生類別)
│   ├── Electronics.h / .cpp  電子產品 (衍生類別)
│   ├── Clothing.h / .cpp     服飾 (衍生類別)
│   ├── Inventory.h / .cpp    庫存管理 (STL 容器 + 檔案 I/O)
│   └── main.cpp              終端機 UI 主程式
└── docs/
    ├── SPECIFICATION.md      規格說明書
    ├── DEVELOPMENT.md        開發流程
    └── report.html           期末報告原始檔 (可轉 PDF)
```

---

## 三、編譯與執行

需求：`g++` (支援 C++14)、`make`

```bash
# 編譯
make

# 執行
./inventory          # 或   make run

# 清除編譯產出
make clean
```

---

## 四、類別繼承架構 (UML 概念圖)

```
              ┌──────────────────────────┐
              │        Product           │  (抽象基底類別)
              │--------------------------│
              │ # id_  # name_           │
              │ # price_  # quantity_    │
              │--------------------------│
              │ + addStock() / removeStock()        │
              │ + totalValue()  (virtual)           │
              │ + category()      = 0  (pure virtual)│
              │ + displayDetail() = 0                │
              │ + serialize()     = 0                │
              └─────────────┬────────────┘
                            │  繼承 (public)
        ┌───────────────────┼────────────────────┐
        ▼                   ▼                     ▼
┌───────────────┐  ┌────────────────┐   ┌────────────────┐
│     Food      │  │  Electronics   │   │    Clothing    │
│---------------│  │----------------│   │----------------│
│ - expiry_     │  │ - warranty_    │   │ - size_        │
│ 有效期限      │  │ 保固月數       │   │ 尺寸           │
└───────────────┘  └────────────────┘   └────────────────┘
```

`Inventory` 以 `std::vector<std::unique_ptr<Product>>` 持有上述物件，
透過基底指標呼叫被覆寫的虛擬函式，即為 **多型 (polymorphism)**。

---

## 五、功能選單

```
   1. 新增商品            7. 修改商品價格
   2. 顯示所有商品        8. 刪除商品
   3. 依類別顯示商品      9. 庫存統計報表
   4. 搜尋商品 (依名稱)  10. 儲存至檔案
   5. 進貨 (增加庫存)    11. 從檔案讀取
   6. 銷售 (減少庫存)     0. 離開系統 (自動存檔)
```

---

## 六、GitHub Repo

https://github.com/jamiechu218/final_project
