#ifndef ELECTRONICS_H
#define ELECTRONICS_H

#include "Product.h"

// =============================================================
//  Electronics (電子產品) — 繼承自 Product
// -------------------------------------------------------------
//  額外屬性：warranty_ 保固月數
// =============================================================
class Electronics : public Product {
private:
    int warranty_;   // 保固月數

public:
    Electronics(int id, const std::string& name, double price, int quantity,
                int warranty);

    int getWarranty() const { return warranty_; }

    std::string category()      const override;
    void        displayDetail() const override;
    std::string serialize()     const override;
};

#endif // ELECTRONICS_H
