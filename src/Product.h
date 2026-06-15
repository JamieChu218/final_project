#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

// =============================================================
//  Product (抽象基底類別 / Abstract Base Class)
// -------------------------------------------------------------
//  這是整個系統繼承架構的最上層。
//  Food / Electronics / Clothing 皆由 Product 繼承而來，
//  並覆寫 (override) category()、displayDetail()、serialize()
//  以展現 C++ 的「多型 (polymorphism)」與「類別繼承」。
// =============================================================
class Product {
protected:
    int         id_;        // 商品編號
    std::string name_;      // 商品名稱
    double      price_;     // 單價
    int         quantity_;  // 庫存數量

public:
    Product(int id, const std::string& name, double price, int quantity);

    // 虛擬解構子：確保以基底指標刪除衍生物件時可正確釋放資源
    virtual ~Product();

    // ---------- 共用 getter ----------
    int                getId()       const { return id_; }
    const std::string& getName()     const { return name_; }
    double             getPrice()    const { return price_; }
    int                getQuantity() const { return quantity_; }

    // ---------- 共用 setter / 操作 ----------
    void setPrice(double price) { price_ = price; }
    void addStock(int amount);          // 進貨
    bool removeStock(int amount);       // 銷售 (庫存不足回傳 false)

    // 庫存總值 = 單價 × 數量 (虛擬函式，衍生類別可視需要覆寫)
    virtual double totalValue() const { return price_ * quantity_; }

    // ---------- 純虛擬函式 (pure virtual) ----------
    // 由衍生類別實作，使 Product 成為抽象類別
    virtual std::string category()      const = 0;  // 類別名稱
    virtual void        displayDetail() const = 0;  // 顯示完整資訊
    virtual std::string serialize()     const = 0;  // 轉成可寫入檔案的字串
};

#endif // PRODUCT_H
