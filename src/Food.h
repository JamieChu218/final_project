#ifndef FOOD_H
#define FOOD_H

#include "Product.h"

// =============================================================
//  Food (食品類) — 繼承自 Product
// -------------------------------------------------------------
//  額外屬性：expiry_ 有效期限 (YYYY-MM-DD)
// =============================================================
class Food : public Product {
private:
    std::string expiry_;   // 有效期限

public:
    Food(int id, const std::string& name, double price, int quantity,
         const std::string& expiry);

    const std::string& getExpiry() const { return expiry_; }

    // 覆寫基底類別的純虛擬函式
    std::string category()      const override;
    void        displayDetail() const override;
    std::string serialize()     const override;
};

#endif // FOOD_H
