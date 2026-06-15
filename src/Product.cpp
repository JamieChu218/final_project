#include "Product.h"

// 建構子：初始化基底類別的共用欄位
Product::Product(int id, const std::string& name, double price, int quantity)
    : id_(id), name_(name), price_(price), quantity_(quantity) {}

// 虛擬解構子 (即使是空的，也要提供定義)
Product::~Product() {}

// 進貨：增加庫存數量
void Product::addStock(int amount) {
    if (amount > 0) {
        quantity_ += amount;
    }
}

// 銷售：減少庫存數量；庫存不足則不動作並回傳 false
bool Product::removeStock(int amount) {
    if (amount > 0 && amount <= quantity_) {
        quantity_ -= amount;
        return true;
    }
    return false;
}
