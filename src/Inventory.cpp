#include "Inventory.h"
#include "Food.h"
#include "Electronics.h"
#include "Clothing.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>

Inventory::Inventory() : nextId_(1001) {}

// 產生新的商品編號 (1001, 1002, ...)
int Inventory::generateId() {
    return nextId_++;
}

void Inventory::addProduct(std::unique_ptr<Product> p) {
    // 確保 nextId_ 永遠大於既有最大編號，避免重號
    if (p->getId() >= nextId_) {
        nextId_ = p->getId() + 1;
    }
    products_.push_back(std::move(p));
}

// 依編號尋找商品；找不到回傳 nullptr
Product* Inventory::findById(int id) const {
    for (const auto& p : products_) {
        if (p->getId() == id) {
            return p.get();
        }
    }
    return nullptr;
}

// 依名稱關鍵字模糊搜尋 (使用 STL std::string::find)
std::vector<Product*> Inventory::searchByName(const std::string& kw) const {
    std::vector<Product*> result;
    for (const auto& p : products_) {
        if (p->getName().find(kw) != std::string::npos) {
            result.push_back(p.get());
        }
    }
    return result;
}

// 依編號刪除 (使用 STL std::remove_if + erase)
bool Inventory::removeById(int id) {
    auto it = std::remove_if(products_.begin(), products_.end(),
                             [id](const std::unique_ptr<Product>& p) {
                                 return p->getId() == id;
                             });
    if (it != products_.end()) {
        products_.erase(it, products_.end());
        return true;
    }
    return false;
}

bool Inventory::sell(int id, int amount) {
    Product* p = findById(id);
    return p && p->removeStock(amount);
}

bool Inventory::restock(int id, int amount) {
    Product* p = findById(id);
    if (!p) return false;
    p->addStock(amount);
    return true;
}

// 顯示全部商品 — 透過基底指標呼叫 displayDetail()，展現「多型」
void Inventory::displayAll() const {
    if (products_.empty()) {
        std::cout << "  (目前沒有任何商品)\n";
        return;
    }
    for (const auto& p : products_) {
        p->displayDetail();   // 依實際型別呼叫對應版本
    }
}

void Inventory::displayByCategory(const std::string& c) const {
    bool found = false;
    for (const auto& p : products_) {
        if (p->category() == c) {
            p->displayDetail();
            found = true;
        }
    }
    if (!found) {
        std::cout << "  (此類別目前沒有商品)\n";
    }
}

// 庫存統計報表：使用 std::map 統計各類別數量與金額
void Inventory::displayReport() const {
    std::map<std::string, int>    countByCat;   // 各類別商品種類數
    std::map<std::string, double> valueByCat;   // 各類別庫存總值

    for (const auto& p : products_) {
        countByCat[p->category()] += 1;
        valueByCat[p->category()] += p->totalValue();
    }

    std::cout << "  -------------------------------------------\n";
    std::cout << "  類別            種類數      庫存總值\n";
    std::cout << "  -------------------------------------------\n";
    for (const auto& kv : countByCat) {
        std::cout << "  " << std::left << std::setw(16) << kv.first
                  << std::setw(12) << kv.second
                  << std::fixed << std::setprecision(1) << valueByCat[kv.first]
                  << "\n";
    }
    std::cout << "  -------------------------------------------\n";
    std::cout << "  商品總種類數: " << products_.size()
              << "    全部庫存總值: " << std::fixed << std::setprecision(1)
              << totalValue() << "\n";
}

double Inventory::totalValue() const {
    double sum = 0.0;
    for (const auto& p : products_) {
        sum += p->totalValue();
    }
    return sum;
}

// ---------- 寫檔 ----------
// 每行一筆，欄位以 '|' 分隔，由各類別的 serialize() 產生
bool Inventory::saveToFile(const std::string& path) const {
    std::ofstream ofs(path);
    if (!ofs) {
        return false;
    }
    for (const auto& p : products_) {
        ofs << p->serialize() << "\n";
    }
    return true;
}

// ---------- 讀檔 ----------
// 依第一欄判斷型別，動態建立對應的衍生類別物件 (工廠模式)
bool Inventory::loadFromFile(const std::string& path) {
    std::ifstream ifs(path);
    if (!ifs) {
        return false;
    }

    products_.clear();
    nextId_ = 1001;
    std::string line;

    while (std::getline(ifs, line)) {
        if (line.empty()) continue;

        // 以 '|' 切割欄位
        std::vector<std::string> f;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, '|')) {
            f.push_back(token);
        }
        if (f.size() < 6) continue;   // 格式不符則略過

        std::string type = f[0];
        int    id    = std::stoi(f[1]);
        std::string name = f[2];
        double price = std::stod(f[3]);
        int    qty   = std::stoi(f[4]);

        if (type == "Food") {
            addProduct(std::make_unique<Food>(id, name, price, qty, f[5]));
        } else if (type == "Electronics") {
            addProduct(std::make_unique<Electronics>(id, name, price, qty,
                                                     std::stoi(f[5])));
        } else if (type == "Clothing") {
            addProduct(std::make_unique<Clothing>(id, name, price, qty, f[5]));
        }
    }
    return true;
}
