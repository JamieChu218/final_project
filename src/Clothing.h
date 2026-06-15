#ifndef CLOTHING_H
#define CLOTHING_H

#include "Product.h"

// =============================================================
//  Clothing (服飾類) — 繼承自 Product
// -------------------------------------------------------------
//  額外屬性：size_ 尺寸 (S / M / L / XL)
// =============================================================
class Clothing : public Product {
private:
    std::string size_;   // 尺寸

public:
    Clothing(int id, const std::string& name, double price, int quantity,
             const std::string& size);

    const std::string& getSize() const { return size_; }

    std::string category()      const override;
    void        displayDetail() const override;
    std::string serialize()     const override;
};

#endif // CLOTHING_H
