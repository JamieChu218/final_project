#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""產生 UML 類別繼承圖 (PNG)，取代原本不易閱讀的 ASCII 樹狀圖。"""
import os
from PIL import Image, ImageDraw, ImageFont

OUT = "/home/user/final_project/docs/diagram_inheritance.png"
FP = "/usr/share/fonts/truetype/wqy/wqy-zenhei.ttc"

def F(sz):
    return ImageFont.truetype(FP, sz)

W, H = 1180, 740
img = Image.new("RGB", (W, H), "#ffffff")
d = ImageDraw.Draw(img)

BORDER = "#2b4c7e"
FILL = "#eef3fb"
TITLEC = "#1f3a63"
INK = "#222222"
LINE = "#4a6fa5"

def center(text, fnt, cx, y, color):
    w = d.textlength(text, font=fnt)
    d.text((cx - w / 2, y), text, font=fnt, fill=color)

def class_box(x0, y0, x1, y1, title, sections, tsize=26, bsize=19):
    d.rectangle([x0, y0, x1, y1], fill=FILL, outline=BORDER, width=3)
    cx = (x0 + x1) / 2
    center(title, F(tsize), cx, y0 + 12, TITLEC)
    y = y0 + 12 + tsize + 12
    for si, sec in enumerate(sections):
        d.line([(x0, y), (x1, y)], fill=BORDER, width=2)
        y += 10
        for ln in sec:
            d.text((x0 + 16, y), ln, font=F(bsize), fill=INK)
            y += bsize + 12
        y += 4

# ---- Product 基底類別 ----
px0, px1 = 310, 870
py0, py1 = 40, 300
class_box(px0, py0, px1, py1, "Product （抽象基底類別）",
          [["id_      name_      price_      quantity_"],
           ["+ category()       = 0   （純虛擬）",
            "+ displayDetail()  = 0   （純虛擬，多型）",
            "+ serialize()      = 0   （純虛擬）"]])

# ---- 繼承連接線 (UML 空心三角形指向基底) ----
cx = (px0 + px1) / 2
bus_y = 370
# 三角形 (指向 Product 底部)
d.polygon([(cx, py1), (cx - 16, py1 + 26), (cx + 16, py1 + 26)],
          outline=BORDER, fill="#ffffff", width=3)
d.line([(cx, py1 + 26), (cx, bus_y)], fill=LINE, width=3)
center("繼承 (public)", F(18), cx + 95, py1 + 18, LINE)

# 子類別中心位置
centers = [205, 595, 975]
top_y = 470
bot_y = 590
bw = 320
# 橫向匯流排
d.line([(centers[0], bus_y), (centers[-1], bus_y)], fill=LINE, width=3)
for c in centers:
    d.line([(c, bus_y), (c, top_y)], fill=LINE, width=3)

# ---- 三個衍生類別 ----
derived = [
    ("Food （食品）", "+ expiry_  有效期限"),
    ("Electronics （電子）", "+ warranty_  保固月數"),
    ("Clothing （服飾）", "+ size_  尺寸"),
]
for c, (title, attr) in zip(centers, derived):
    class_box(c - bw / 2, top_y, c + bw / 2, bot_y, title, [[attr]],
              tsize=23, bsize=19)

# ---- 底部說明 ----
note = ("Inventory 以 std::vector<std::unique_ptr<Product>> 持有所有商品，"
        "透過基底指標呼叫 displayDetail()，由實際型別決定輸出格式 → 多型")
center(note, F(18), W / 2, 660, "#555555")

img.save(OUT)
print("saved", OUT, img.size)
