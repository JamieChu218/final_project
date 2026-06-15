// =============================================================
//  商店庫存管理系統 (Store Inventory Management System)
//  C++ 期末專題 — main.cpp
// -------------------------------------------------------------
//  展示重點：
//    1. 類別繼承 / 多型   Product -> Food / Electronics / Clothing
//    2. 檔案讀寫           Inventory::saveToFile / loadFromFile
//    3. STL 類別庫         vector / map / unique_ptr / string / algorithm
//    4. 終端機 UI          menu-driven 文字操作介面
// =============================================================
#include "Inventory.h"
#include "Food.h"
#include "Electronics.h"
#include "Clothing.h"

#include <iostream>
#include <limits>
#include <string>
#include <memory>
#include <cstdlib>   // system("cls") / system("clear")

static const std::string DATA_FILE = "data/inventory.txt";

// ---------- 共用輸入工具 ----------

// 讀取整數，含基本錯誤處理
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "  輸入錯誤，請輸入數字。\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "  輸入錯誤，請輸入數字。\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
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
}

// ---------- 各功能實作 ----------

void doAddProduct(Inventory& inv) {
    std::cout << "\n--- 新增商品 ---\n";
    std::cout << "  請選擇類別：1) 食品  2) 電子產品  3) 服飾\n";
    int type = readInt("  類別: ");

    std::string name = readLine("  商品名稱: ");
    double price = readDouble("  單價: ");
    int qty = readInt("  庫存數量: ");
    int id = inv.generateId();

    if (type == 1) {
        std::string exp = readLine("  有效期限 (YYYY-MM-DD): ");
        inv.addProduct(std::make_unique<Food>(id, name, price, qty, exp));
    } else if (type == 2) {
        int w = readInt("  保固月數: ");
        inv.addProduct(std::make_unique<Electronics>(id, name, price, qty, w));
    } else if (type == 3) {
        std::string size = readLine("  尺寸 (S/M/L/XL): ");
        inv.addProduct(std::make_unique<Clothing>(id, name, price, qty, size));
    } else {
        std::cout << "  類別錯誤，取消新增。\n";
        return;
    }
    std::cout << "  >> 新增成功！商品編號為 " << id << "\n";
}

void doSearch(const Inventory& inv) {
    std::string kw = readLine("\n  請輸入名稱關鍵字: ");
    auto results = inv.searchByName(kw);
    std::cout << "  找到 " << results.size() << " 筆結果：\n";
    for (Product* p : results) {
        p->displayDetail();
    }
}

void doByCategory(const Inventory& inv) {
    std::cout << "\n  選擇類別：1) 食品  2) 電子產品  3) 服飾\n";
    int t = readInt("  類別: ");
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
        int choice = readInt("請輸入選項: ");

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
                int id = readInt("\n  商品編號: ");
                int amt = readInt("  進貨數量: ");
                std::cout << (inv.restock(id, amt) ? "  >> 進貨成功。\n"
                                                   : "  >> 找不到該商品。\n");
                break;
            }
            case 6: {
                int id = readInt("\n  商品編號: ");
                int amt = readInt("  銷售數量: ");
                std::cout << (inv.sell(id, amt) ? "  >> 銷售成功。\n"
                                                : "  >> 銷售失敗 (商品不存在或庫存不足)。\n");
                break;
            }
            case 7: {
                int id = readInt("\n  商品編號: ");
                Product* p = inv.findById(id);
                if (p) {
                    double np = readDouble("  新單價: ");
                    p->setPrice(np);
                    std::cout << "  >> 價格已更新。\n";
                } else {
                    std::cout << "  >> 找不到該商品。\n";
                }
                break;
            }
            case 8: {
                int id = readInt("\n  欲刪除的商品編號: ");
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
                // 離開前自動存檔
                inv.saveToFile(DATA_FILE);
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
