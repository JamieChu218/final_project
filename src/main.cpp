// =============================================================
//  商店庫存管理系統 (Store Inventory Management System)
//  C++ 期末專題 — main.cpp
// -------------------------------------------------------------
//  展示重點：
//    1. 類別繼承 / 多型   Product -> Food / Electronics / Clothing
//    2. 檔案讀寫           Inventory::saveToFile / loadFromFile
//    3. STL 類別庫         vector / map / unique_ptr / string / algorithm
//    4. 終端機 UI          menu-driven 文字操作介面 (刷新式單頁)
// =============================================================
#include "Inventory.h"
#include "Food.h"
#include "Electronics.h"
#include "Clothing.h"

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdlib>   // system("cls") / system("clear")

#ifdef _WIN32
#include <windows.h> // SetConsoleOutputCP：讓 Windows 主控台正確顯示 UTF-8 繁體中文
#endif

static const std::string DATA_FILE = "data/inventory.txt";
static const std::string CANCEL = "q";   // 在任何輸入步驟輸入 q 即可取消目前動作

// ============================================================
//  輸入工具 (含防呆 + 取消機制)
// ============================================================

// 去除字串前後的空白字元
std::string trim(const std::string& s) {
    const std::string ws = " \t\r\n";
    size_t a = s.find_first_not_of(ws);
    if (a == std::string::npos) return "";
    size_t b = s.find_last_not_of(ws);
    return s.substr(a, b - a + 1);
}

// 讀取一行文字。
//   回傳 false  -> 使用者輸入 q，代表「取消目前動作」
//   allowEmpty=false 時，空白(只按 Enter)會被擋下並重新詢問 (防呆)
bool askLine(const std::string& prompt, std::string& out, bool allowEmpty = false) {
    while (true) {
        std::cout << prompt;
        std::string s;
        if (!std::getline(std::cin, s)) {   // 遇到 EOF，視為取消
            return false;
        }
        std::string t = trim(s);
        if (t == CANCEL || t == "Q") {
            return false;                   // 取消
        }
        if (t.empty() && !allowEmpty) {
            std::cout << "  ※ 輸入不可空白，請重新輸入（或輸入 q 取消）\n";
            continue;                        // 防呆：擋掉單純按 Enter
        }
        out = t;
        return true;
    }
}

// 讀取整數，含防呆與取消；回傳 false 代表取消
bool askInt(const std::string& prompt, int& out) {
    while (true) {
        std::string s;
        if (!askLine(prompt, s)) return false;
        try {
            size_t pos;
            int v = std::stoi(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("trailing");
            out = v;
            return true;
        } catch (...) {
            std::cout << "  ※ 請輸入有效的整數（或輸入 q 取消）\n";
        }
    }
}

// 讀取浮點數，含防呆與取消；回傳 false 代表取消
bool askDouble(const std::string& prompt, double& out) {
    while (true) {
        std::string s;
        if (!askLine(prompt, s)) return false;
        try {
            size_t pos;
            double v = std::stod(s, &pos);
            if (pos != s.size()) throw std::invalid_argument("trailing");
            out = v;
            return true;
        } catch (...) {
            std::cout << "  ※ 請輸入有效的數字（或輸入 q 取消）\n";
        }
    }
}

// 清除整個終端機畫面 (跨平台)：達成「刷新式單頁」顯示效果
void clearScreen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

// 暫停，等待使用者按 Enter，避免結果一閃即逝就被清除
void pause() {
    std::cout << "\n>> 按 Enter 鍵繼續...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

// 提示已取消目前動作
void cancelled() {
    std::cout << "  >> 已取消目前動作，返回主選單。\n";
}

// ---------- 選單畫面 ----------
void printMenu() {
    std::cout << "\n";
    std::cout << "==================================================\n";
    std::cout << "            商店庫存管理系統  v1.0                \n";
    std::cout << "==================================================\n";
    std::cout << "   1. 新增商品\n";
    std::cout << "   2. 顯示所有商品\n";
    std::cout << "   3. 依類別顯示商品\n";
    std::cout << "   4. 搜尋商品 (依名稱)\n";
    std::cout << "   5. 進貨 (增加庫存)\n";
    std::cout << "   6. 銷售 (減少庫存)\n";
    std::cout << "   7. 修改商品價格\n";
    std::cout << "   8. 刪除商品\n";
    std::cout << "   9. 庫存統計報表\n";
    std::cout << "  10. 儲存至檔案\n";
    std::cout << "  11. 從檔案讀取\n";
    std::cout << "   0. 離開系統\n";
    std::cout << "==================================================\n";
    std::cout << "  （提示：操作過程中輸入 q 可隨時取消）\n";
}

// ---------- 各功能實作 ----------

void doAddProduct(Inventory& inv) {
    std::cout << "\n--- 新增商品（任何步驟輸入 q 可取消）---\n";
    std::cout << "  請選擇類別：1) 食品  2) 電子產品  3) 服飾\n";

    int type;
    if (!askInt("  類別: ", type)) { cancelled(); return; }
    if (type < 1 || type > 3) {
        std::cout << "  類別錯誤，取消新增。\n";
        return;
    }

    std::string name;
    if (!askLine("  商品名稱: ", name)) { cancelled(); return; }
    double price;
    if (!askDouble("  單價: ", price)) { cancelled(); return; }
    int qty;
    if (!askInt("  庫存數量: ", qty)) { cancelled(); return; }

    if (type == 1) {
        std::string exp;
        if (!askLine("  有效期限 (YYYY-MM-DD): ", exp)) { cancelled(); return; }
        int id = inv.generateId();
        inv.addProduct(std::make_unique<Food>(id, name, price, qty, exp));
        std::cout << "  >> 新增成功！商品編號為 " << id << "\n";
    } else if (type == 2) {
        int w;
        if (!askInt("  保固月數: ", w)) { cancelled(); return; }
        int id = inv.generateId();
        inv.addProduct(std::make_unique<Electronics>(id, name, price, qty, w));
        std::cout << "  >> 新增成功！商品編號為 " << id << "\n";
    } else {
        std::string size;
        if (!askLine("  尺寸 (S/M/L/XL): ", size)) { cancelled(); return; }
        int id = inv.generateId();
        inv.addProduct(std::make_unique<Clothing>(id, name, price, qty, size));
        std::cout << "  >> 新增成功！商品編號為 " << id << "\n";
    }
}

void doSearch(const Inventory& inv) {
    std::string kw;
    if (!askLine("\n  請輸入名稱關鍵字（q 取消）: ", kw)) { cancelled(); return; }
    auto results = inv.searchByName(kw);
    std::cout << "  找到 " << results.size() << " 筆結果：\n";
    for (Product* p : results) {
        p->displayDetail();
    }
}

void doByCategory(const Inventory& inv) {
    std::cout << "\n  選擇類別：1) 食品  2) 電子產品  3) 服飾\n";
    int t;
    if (!askInt("  類別（q 取消）: ", t)) { cancelled(); return; }
    std::string c = (t == 1) ? "Food" : (t == 2) ? "Electronics"
                  : (t == 3) ? "Clothing" : "";
    if (c.empty()) {
        std::cout << "  類別錯誤。\n";
        return;
    }
    std::cout << "\n";
    inv.displayByCategory(c);
}

int main() {
#ifdef _WIN32
    // 讓 Windows 主控台以 UTF-8 顯示 / 讀取，正確呈現繁體中文，避免亂碼
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    Inventory inv;

    // 啟動時嘗試自動載入既有資料
    if (inv.loadFromFile(DATA_FILE)) {
        std::cout << "已從 " << DATA_FILE << " 載入 " << inv.size() << " 筆商品資料。\n";
    } else {
        std::cout << "找不到既有資料檔，將以空白庫存開始。\n";
    }
    pause();   // 讓使用者先看到載入結果，再進入主畫面

    bool running = true;
    while (running) {
        clearScreen();                       // 每次重畫前先清空畫面 (刷新式單頁)
        printMenu();

        int choice;
        if (!askInt("請輸入選項: ", choice)) {
            // 在主選單輸入 q：直接重新整理選單畫面
            continue;
        }

        switch (choice) {
            case 1:
                doAddProduct(inv);
                break;
            case 2:
                std::cout << "\n--- 所有商品 ---\n";
                inv.displayAll();
                break;
            case 3:
                doByCategory(inv);
                break;
            case 4:
                doSearch(inv);
                break;
            case 5: {
                int id, amt;
                if (!askInt("\n  商品編號（q 取消）: ", id)) { cancelled(); break; }
                if (!askInt("  進貨數量: ", amt))          { cancelled(); break; }
                std::cout << (inv.restock(id, amt) ? "  >> 進貨成功。\n"
                                                   : "  >> 找不到該商品。\n");
                break;
            }
            case 6: {
                int id, amt;
                if (!askInt("\n  商品編號（q 取消）: ", id)) { cancelled(); break; }
                if (!askInt("  銷售數量: ", amt))          { cancelled(); break; }
                std::cout << (inv.sell(id, amt) ? "  >> 銷售成功。\n"
                                                : "  >> 銷售失敗 (商品不存在或庫存不足)。\n");
                break;
            }
            case 7: {
                int id;
                if (!askInt("\n  商品編號（q 取消）: ", id)) { cancelled(); break; }
                Product* p = inv.findById(id);
                if (p) {
                    double np;
                    if (!askDouble("  新單價: ", np)) { cancelled(); break; }
                    p->setPrice(np);
                    std::cout << "  >> 價格已更新。\n";
                } else {
                    std::cout << "  >> 找不到該商品。\n";
                }
                break;
            }
            case 8: {
                int id;
                if (!askInt("\n  欲刪除的商品編號（q 取消）: ", id)) { cancelled(); break; }
                std::cout << (inv.removeById(id) ? "  >> 已刪除。\n"
                                                 : "  >> 找不到該商品。\n");
                break;
            }
            case 9:
                std::cout << "\n--- 庫存統計報表 ---\n";
                inv.displayReport();
                break;
            case 10:
                std::cout << (inv.saveToFile(DATA_FILE)
                                  ? "\n  >> 已儲存至 " + DATA_FILE + "\n"
                                  : "\n  >> 儲存失敗。\n");
                break;
            case 11:
                std::cout << (inv.loadFromFile(DATA_FILE)
                                  ? "\n  >> 已從檔案重新載入。\n"
                                  : "\n  >> 讀取失敗。\n");
                break;
            case 0:
                running = false;
                inv.saveToFile(DATA_FILE);    // 離開前自動存檔
                std::cout << "\n資料已自動儲存，感謝使用，再見！\n";
                break;
            default:
                std::cout << "  無效的選項，請重新輸入。\n";
        }

        // 動作完成後暫停，等使用者按 Enter，下一輪迴圈才會清空並重畫選單
        if (choice != 0) {
            pause();
        }
    }
    return 0;
}
