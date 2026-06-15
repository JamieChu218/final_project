#include "Food.h"
#include <iostream>
#include <iomanip>

Food::Food(int id, const std::string& name, double price, int quantity,
           const std::string& expiry)
    : Product(id, name, price, quantity), expiry_(expiry) {}

std::string Food::category() const {
    return "Food";
}

// 多型：以食品專屬格式顯示資訊
void Food::displayDetail() const {
    std::cout << std::left
              << "  [食品] 編號:" << std::setw(4) << id_
              << " 名稱:" << std::setw(12) << name_
              << " 單價:" << std::setw(8) << std::fixed << std::setprecision(1) << price_
              << " 庫存:" << std::setw(5) << quantity_
              << " 有效期限:" << expiry_
              << "\n";
}

// 序列化格式：Food|id|name|price|quantity|expiry
std::string Food::serialize() const {
    return "Food|" + std::to_string(id_) + "|" + name_ + "|" +
           std::to_string(price_) + "|" + std::to_string(quantity_) + "|" +
           expiry_;
}
