#include "Electronics.h"
#include <iostream>
#include <iomanip>

Electronics::Electronics(int id, const std::string& name, double price,
                         int quantity, int warranty)
    : Product(id, name, price, quantity), warranty_(warranty) {}

std::string Electronics::category() const {
    return "Electronics";
}

void Electronics::displayDetail() const {
    std::cout << std::left
              << "  [電子] 編號:" << std::setw(4) << id_
              << " 名稱:" << std::setw(12) << name_
              << " 單價:" << std::setw(8) << std::fixed << std::setprecision(1) << price_
              << " 庫存:" << std::setw(5) << quantity_
              << " 保固:" << warranty_ << " 個月"
              << "\n";
}

// 序列化格式：Electronics|id|name|price|quantity|warranty
std::string Electronics::serialize() const {
    return "Electronics|" + std::to_string(id_) + "|" + name_ + "|" +
           std::to_string(price_) + "|" + std::to_string(quantity_) + "|" +
           std::to_string(warranty_);
}
