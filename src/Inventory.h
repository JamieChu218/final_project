#ifndef INVENTORY_H
#define INVENTORY_H

#include "Product.h"
#include <vector>
#include <memory>
#include <string>

// =============================================================
//  Inventory (庫存管理) — 系統的核心資料管理類別
// -------------------------------------------------------------
//  使用 STL 容器：
//    std::vector<std::unique_ptr<Product>>  儲存所有商品
//  以 unique_ptr 持有 Product 基底指標，達成多型 +
//  自動記憶體管理 (RAII)。
// =============================================================
class Inventory {
private:
    std::vector<std::unique_ptr<Product>> products_;  // STL 容器
    int nextId_;                                      // 自動編號

public:
    Inventory();

    // ---------- 新增 ----------
    // 將動態配置的 Product (Food/Electronics/Clothing) 加入庫存
    void addProduct(std::unique_ptr<Product> p);

    // 自動配發下一個商品編號
    int  generateId();

    // ---------- 查詢 ----------
    Product*                    findById(int id) const;        // 依編號
    std::vector<Product*>       searchByName(const std::string& kw) const;  // 模糊搜尋
    const std::vector<std::unique_ptr<Product>>& all() const { return products_; }
    bool   empty() const { return products_.empty(); }
    size_t size()  const { return products_.size(); }

    // ---------- 操作 ----------
    bool removeById(int id);                       // 刪除
    bool sell(int id, int amount);                 // 銷售
    bool restock(int id, int amount);              // 進貨

    // ---------- 顯示 / 統計 ----------
    void displayAll() const;                       // 顯示全部 (多型)
    void displayByCategory(const std::string& c) const;
    void displayReport() const;                    // 庫存統計報表
    double totalValue() const;                     // 全部庫存總值

    // ---------- 檔案 I/O ----------
    bool saveToFile(const std::string& path) const;   // 寫檔
    bool loadFromFile(const std::string& path);       // 讀檔
};

#endif // INVENTORY_H
