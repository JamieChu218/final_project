#include "Clothing.h"
#include <iostream>
#include <iomanip>

Clothing::Clothing(int id, const std::string& name, double price, int quantity,
                   const std::string& size)
    : Product(id, name, price, quantity), size_(size) {}

std::string Clothing::category() const {
    return "Clothing";
}

void Clothing::displayDetail() const {
    std::cout << std::left
              << "  [服飾] 編號:" << std::setw(4) << id_
              << " 名稱:" << std::setw(12) << name_
              << " 單價:" << std::setw(8) << std::fixed << std::setprecision(1) << price_
              << " 庫存:" << std::setw(5) << quantity_
              << " 尺寸:" << size_
              << "\n";
}

// 序列化格式：Clothing|id|name|price|quantity|size
std::string Clothing::serialize() const {
    return "Clothing|" + std::to_string(id_) + "|" + name_ + "|" +
           std::to_string(price_) + "|" + std::to_string(quantity_) + "|" +
           size_;
}
