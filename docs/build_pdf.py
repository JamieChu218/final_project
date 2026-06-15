#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""以 PIL 將期末報告排版為 A4 多頁 PDF。"""
import os
from PIL import Image, ImageDraw, ImageFont

DOCS = "/home/user/final_project/docs"
SHOTS = os.path.join(DOCS, "screenshots")
OUT = "/home/user/final_project/report/4B4G0039.pdf"
os.makedirs(os.path.dirname(OUT), exist_ok=True)

FP = "/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc"
def font(sz, mono=False, idx=None):
    return ImageFont.truetype(FP, sz, index=(1 if mono else 0) if idx is None else idx)

# A4 @150dpi
W, H = 1240, 1754
MX = 110                      # 左右邊界
CW = W - 2 * MX              # 內容寬度
BG = "#ffffff"
INK = "#222222"
BLUE = "#2b4c7e"
LBLUE = "#4a6fa5"

F_BODY = font(26)
F_H1 = font(60)
F_H2 = font(38)
F_H3 = font(30)
F_TBL = font(23)
F_TBLH = font(23)
F_CAP = font(22)
F_CODE = font(22, mono=True)
F_SMALL = font(22)

pages = []
img = None
draw = None
y = 0

def new_page():
    global img, draw, y
    if img is not None:
        pages.append(img)
    img = Image.new("RGB", (W, H), BG)
    draw = ImageDraw.Draw(img)
    y = MX

def ensure(space):
    if y + space > H - MX:
        new_page()

def is_cjk(ch):
    o = ord(ch)
    return (0x2E80 <= o <= 0x9FFF or 0xAC00 <= o <= 0xD7A3 or
            0xF900 <= o <= 0xFAFF or 0x3000 <= o <= 0x303F or
            0xFF00 <= o <= 0xFFEF)

def tokenize(text):
    toks, buf = [], ""
    for ch in text:
        if ch == " ":
            if buf: toks.append(buf); buf = ""
            toks.append(" ")
        elif is_cjk(ch):
            if buf: toks.append(buf); buf = ""
            toks.append(ch)
        else:
            buf += ch
    if buf: toks.append(buf)
    return toks

def wrap(text, fnt, maxw):
    lines, cur = [], ""
    for tok in tokenize(text):
        trial = cur + tok
        if draw.textlength(trial, font=fnt) <= maxw or cur == "":
            cur = trial
        else:
            lines.append(cur.rstrip() if cur.endswith(" ") else cur)
            cur = "" if tok == " " else tok
    if cur:
        lines.append(cur)
    return lines

def para(text, fnt=F_BODY, color=INK, lh=40, gap=10, indent=0):
    global y
    for ln in wrap(text, fnt, CW - indent):
        ensure(lh)
        draw.text((MX + indent, y), ln, font=fnt, fill=color)
        y += lh
    y += gap

def heading2(text):
    global y
    ensure(80)
    y += 18
    draw.text((MX, y), text, font=F_H2, fill=BLUE)
    y += 52
    draw.line([(MX, y), (W - MX, y)], fill=LBLUE, width=4)
    y += 22

def heading3(text):
    global y
    ensure(60)
    y += 8
    draw.text((MX, y), text, font=F_H3, fill=BLUE)
    y += 46

def spacer(px):
    global y
    y += px

def code_block(lines):
    global y
    pad = 16
    lh = 32
    h = pad * 2 + lh * len(lines)
    ensure(h + 10)
    draw.rectangle([MX, y, W - MX, y + h], fill="#f4f4f4", outline="#cccccc")
    yy = y + pad
    for ln in lines:
        draw.text((MX + pad, yy), ln, font=F_CODE, fill="#333333")
        yy += lh
    y += h + 16

def table(rows, widths, header=True):
    """rows: list of list[str]; widths: 比例 list 加總=1"""
    global y
    colw = [int(CW * w) for w in widths]
    pad = 10
    lh = 30
    for ri, row in enumerate(rows):
        # 計算每格折行
        cells = [wrap(str(c), F_TBL, colw[ci] - 2 * pad) for ci, c in enumerate(row)]
        rh = max(len(c) for c in cells) * lh + 2 * pad
        ensure(rh)
        x = MX
        fill = "#dde6f2" if (header and ri == 0) else "#ffffff"
        for ci, cell in enumerate(cells):
            draw.rectangle([x, y, x + colw[ci], y + rh], fill=fill, outline="#999999")
            yy = y + pad
            for ln in cell:
                draw.text((x + pad, yy), ln, font=F_TBL, fill=INK)
                yy += lh
            x += colw[ci]
        y += rh
    y += 16

def image(path, scale=1.0, center=True):
    global y
    im = Image.open(path).convert("RGB")
    maxw = CW
    w = int(im.width * scale)
    if w > maxw:
        w = maxw
    h = int(im.height * w / im.width)
    ensure(h + 12)
    im = im.resize((w, h))
    x = MX + (CW - w) // 2 if center else MX
    img.paste(im, (x, y))
    draw.rectangle([x, y, x + w, y + h], outline="#888888")
    y += h + 16

# ===================== 封面 =====================
new_page()
y = 360
def ctext(t, fnt, color, gap):
    global y
    w = draw.textlength(t, font=fnt)
    draw.text(((W - w) // 2, y), t, font=fnt, fill=color)
    y += gap
ctext("物件導向程式設計　期末專題報告", font(30), "#555555", 120)
ctext("商店庫存管理系統", F_H1, "#1f3a63", 80)
ctext("Store Inventory Management System", font(26), "#777777", 150)
# 資訊框
bx0, bx1 = 360, 880
by0 = y
info = ["指導老師：陳定宏",
        "學　號：4B4G0039",
        "姓　名：江金桔",
        "製作日期：2026 / 06 / 15"]
bh = 40 + len(info) * 56
draw.rectangle([bx0, by0, bx1, by0 + bh], outline=LBLUE, width=3)
yy = by0 + 30
for line in info:
    draw.text((bx0 + 60, yy), line, font=font(28), fill=INK)
    yy += 56
y = by0 + bh + 120
ctext("GitHub Repo", font(26), "#555555", 44)
ctext("https://github.com/jamiechu218/final_project", font(26), "#1f3a63", 60)

# ===================== 內文 =====================
new_page()
heading2("一、專題簡介")
para("本專題以 C++ 物件導向設計一套於終端機 (Terminal) 操作的商店庫存管理系統。"
     "商店中的商品分為「食品、電子產品、服飾」三大類，使用者可透過文字選單進行新增、"
     "查詢、進貨、銷售、修改、刪除與統計，所有資料以檔案永久保存，下次開啟程式時自動載入。")
heading3("對應題目六大要求")
table([
    ["要求", "本專題實作方式"],
    ["1. 呈現 C++ 類別繼承", "抽象基底類別 Product 衍生 Food、Electronics、Clothing，並以虛擬函式達成多型"],
    ["2. 讀檔與寫檔", "Inventory::saveToFile() 寫檔、loadFromFile() 讀檔；啟動自動載入、離開自動存檔"],
    ["3. 使用 STL 類別庫", "vector、map、unique_ptr、string、algorithm、sstream"],
    ["4. 終端機 UI 介面", "main.cpp 提供 12 項功能的文字選單與輸入錯誤處理"],
    ["5. 規格與開發流程", "docs/SPECIFICATION.md、docs/DEVELOPMENT.md"],
    ["6. 完整說明文件", "README、規格書、開發流程與本份報告"],
], [0.30, 0.70])

heading2("二、類別繼承架構（重點）")
para("系統核心是一個三層的繼承架構。Product 為抽象基底類別，定義所有商品共用的屬性與"
     "三個純虛擬函式；三個衍生類別各自擁有專屬屬性並覆寫虛擬函式：")
code_block([
    "        Product  (抽象基底類別)",
    "        |- id_  name_  price_  quantity_",
    "        |- category()      = 0   <- 純虛擬",
    "        |- displayDetail() = 0   <- 純虛擬 (多型)",
    "        |- serialize()     = 0   <- 純虛擬",
    "                 |  public 繼承",
    "      +----------+-----------+",
    "    Food     Electronics   Clothing",
    "  (有效期限)   (保固月數)    (尺寸)",
])
para("Inventory 以 std::vector<std::unique_ptr<Product>> 持有所有商品，僅透過基底指標"
     "呼叫 displayDetail()，即由實際型別決定輸出格式，這就是多型 (polymorphism)。")

# 第三頁：功能說明
new_page()
heading2("三、功能說明")
table([
    ["編號", "功能", "說明"],
    ["1", "新增商品", "選擇類別後輸入名稱、單價、數量與專屬欄位，自動配發編號"],
    ["2", "顯示所有商品", "依實際類別以不同格式列出（多型展示）"],
    ["3", "依類別顯示", "只列出食品 / 電子 / 服飾其中一類"],
    ["4", "搜尋商品", "可依名稱關鍵字或依商品編號搜尋"],
    ["5", "進貨", "對指定編號增加庫存"],
    ["6", "銷售", "減少庫存；庫存不足則拒絕，避免負庫存"],
    ["7", "修改價格", "變更指定商品單價"],
    ["8", "刪除商品", "以 std::remove_if 移除指定商品"],
    ["9", "統計報表", "以 std::map 統計各類別種類數與庫存總值"],
    ["10", "儲存至檔案", "將庫存寫入 data/inventory.txt"],
    ["11", "從檔案讀取", "重新由檔案載入庫存"],
    ["0", "離開系統", "自動存檔後結束"],
], [0.10, 0.22, 0.68])

# 執行畫面
new_page()
heading2("四、程式執行畫面與說明")
heading3("畫面 1：啟動載入資料 + 主選單（刷新式單頁 UI）")
para("程式啟動時自動由 data/inventory.txt 讀入 6 筆商品，按 Enter 後進入主選單。"
     "採刷新式單頁介面，每次操作後會清空畫面、重畫乾淨的選單。",
     fnt=F_CAP, color="#555555", lh=32)
image(os.path.join(SHOTS, "01_startup_menu.png"), scale=0.60)
heading3("畫面 2：銷售 — 列出清單用「項次」選取")
para("進貨 / 銷售 / 改價 / 刪除時會先列出商品清單，使用者只需輸入「項次」(1,2,3…) 即可選取，"
     "不必記憶商品編號；清單同時以不同格式顯示三種商品，即為多型。完成後回報最新庫存。",
     fnt=F_CAP, color="#555555", lh=32)
image(os.path.join(SHOTS, "02_sell_select.png"), scale=0.60)

new_page()
heading3("畫面 3：搜尋 — 可依名稱或依商品編號")
para("搜尋商品時可選擇「依名稱關鍵字」或「依商品編號」兩種方式，圖中示範以編號 1004 找到行動電源。",
     fnt=F_CAP, color="#555555", lh=32)
image(os.path.join(SHOTS, "03_search.png"), scale=0.66)
heading3("畫面 4：防呆與取消機制")
para("必填欄位若只按 Enter（空白）會被擋下並要求重新輸入（防呆）；"
     "任何輸入步驟輸入 q 可隨時取消目前動作並返回主選單。",
     fnt=F_CAP, color="#555555", lh=32)
image(os.path.join(SHOTS, "04_foolproof_cancel.png"), scale=0.66)

# 編譯與結語
new_page()
heading2("五、編譯與執行方式")
code_block([
    "$ make          # 以 g++ (C++14) 編譯, -Wall -Wextra 無警告",
    "$ ./inventory   # 執行 (或 make run)",
    "$ make clean    # 清除編譯產物",
])
heading2("六、結語")
para("本專題完整實踐了 C++ 的類別繼承與多型、STL 容器、檔案讀寫以及終端機 UI。"
     "透過抽象基底類別與虛擬函式的設計，未來若要新增商品類別（例如「書籍」），"
     "只需新增一個衍生類別並實作三個虛擬函式即可，不必改動既有程式，"
     "展現了物件導向「易擴充、易維護」的優點。")
spacer(30)
ctext = None
y += 10
t = "完整原始碼：https://github.com/jamiechu218/final_project"
w = draw.textlength(t, font=F_BODY)
draw.text(((W - w) // 2, y), t, font=F_BODY, fill=BLUE)

# 收尾輸出
pages.append(img)

import img2pdf, tempfile, glob
tmpdir = tempfile.mkdtemp()
png_paths = []
for i, pg in enumerate(pages):
    p = os.path.join(tmpdir, f"page_{i:02d}.png")
    pg.save(p, "PNG")
    png_paths.append(p)

# A4 版面 (150 dpi) -> 以 img2pdf 指定頁面尺寸為 A4
a4 = (img2pdf.mm_to_pt(210), img2pdf.mm_to_pt(297))
layout = img2pdf.get_layout_fun(a4)
with open(OUT, "wb") as f:
    f.write(img2pdf.convert(png_paths, layout_fun=layout))

for p in png_paths:
    os.remove(p)
os.rmdir(tmpdir)
print("PDF saved:", OUT, "pages:", len(pages))
