#include "esp.h"

void RenderBox(ImDrawList* draw, const Vector2& head, const Vector2& feet, ImU32 color, int boxType) {
    float height = feet.y - head.y;
    if (height < 1.0f) height = 1.0f;
    float width = height * 0.6f;
    float x = (head.x + feet.x) * 0.5f - width * 0.5f;
    float cx = (head.x + feet.x) * 0.5f;
    float cy = (head.y + feet.y) * 0.5f;
    ImVec2 tl(x, head.y);
    ImVec2 tr(x + width, head.y);
    ImVec2 bl(x, feet.y);
    ImVec2 br(x + width, feet.y);
    ImVec2 center(cx, cy);
    float th = 1.0f;

    if (boxType == 1) {
        draw->AddRect(tl, br, color, 0.0f, 0, th);
    } else if (boxType == 2) {
        float len = width * 0.2f;
        draw->AddLine(tl, ImVec2(tl.x + len, tl.y), color, th);
        draw->AddLine(tl, ImVec2(tl.x, tl.y + len), color, th);
        draw->AddLine(tr, ImVec2(tr.x - len, tr.y), color, th);
        draw->AddLine(tr, ImVec2(tr.x, tr.y + len), color, th);
        draw->AddLine(bl, ImVec2(bl.x + len, bl.y), color, th);
        draw->AddLine(bl, ImVec2(bl.x, bl.y - len), color, th);
        draw->AddLine(br, ImVec2(br.x - len, br.y), color, th);
        draw->AddLine(br, ImVec2(br.x, br.y - len), color, th);
    } else if (boxType == 3) {
        draw->AddRectFilled(tl, br, IM_COL32(0, 0, 0, 80), 0.0f, 0);
        draw->AddRect(tl, br, color, 0.0f, 0, th);
    } else if (boxType == 4) {
        draw->AddRect(tl, br, color, 6.0f, 0, th);
    } else if (boxType == 5) {
        float ext = width * 0.15f;
        draw->AddLine(ImVec2(cx, head.y), ImVec2(cx, head.y + ext), color, th);
        draw->AddLine(ImVec2(cx, feet.y), ImVec2(cx, feet.y - ext), color, th);
        draw->AddLine(ImVec2(x, cy), ImVec2(x + ext, cy), color, th);
        draw->AddLine(ImVec2(x + width, cy), ImVec2(x + width - ext, cy), color, th);
        draw->AddLine(ImVec2(cx - 4, cy), ImVec2(cx + 4, cy), color, th);
        draw->AddLine(ImVec2(cx, cy - 4), ImVec2(cx, cy + 4), color, th);
    } else if (boxType == 6) {
        int steps = 16;
        int baseA = 20;
        int rangeA = 80 - baseA;
        for (int i = 0; i < steps; i++) {
            float t0 = (float)i / steps;
            float t1 = (float)(i + 1) / steps;
            float y0 = head.y + height * t0;
            float y1 = head.y + height * t1;
            int a = baseA + (int)(rangeA * t0);
            draw->AddRectFilled(ImVec2(x, y0), ImVec2(x + width, y1),
                IM_COL32((color >> 0) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, a), 0.0f, 0);
        }
        draw->AddRect(tl, br, color, 0.0f, 0, th);
    } else if (boxType == 7) {
        float lenH = width * 0.4f;
        float lenV = height * 0.25f;
        float lTh = th * 1.5f;
        draw->AddLine(tl, ImVec2(tl.x + lenH, tl.y), color, lTh);
        draw->AddLine(tl, ImVec2(tl.x, tl.y + lenV), color, lTh);
        draw->AddLine(tr, ImVec2(tr.x - lenH, tr.y), color, lTh);
        draw->AddLine(tr, ImVec2(tr.x, tr.y + lenV), color, lTh);
        draw->AddLine(bl, ImVec2(bl.x + lenH, bl.y), color, lTh);
        draw->AddLine(bl, ImVec2(bl.x, bl.y - lenV), color, lTh);
        draw->AddLine(br, ImVec2(br.x - lenH, br.y), color, lTh);
        draw->AddLine(br, ImVec2(br.x, br.y - lenV), color, lTh);
    } else if (boxType == 8) {
        draw->AddRect(tl, br, color, 0.0f, 0, th * 2.0f);
        draw->AddRect(ImVec2(tl.x + 3, tl.y + 3), ImVec2(br.x - 3, br.y - 3),
            color, 0.0f, 0, th * 0.5f);
    } else if (boxType == 9) {
        draw->AddCircleFilled(tl, 2.5f, color, 8);
        draw->AddCircleFilled(tr, 2.5f, color, 8);
        draw->AddCircleFilled(bl, 2.5f, color, 8);
        draw->AddCircleFilled(br, 2.5f, color, 8);
        draw->AddCircleFilled(ImVec2(cx, head.y), 2.0f, color, 8);
        draw->AddCircleFilled(ImVec2(cx, feet.y), 2.0f, color, 8);
        draw->AddCircleFilled(ImVec2(x, cy), 2.0f, color, 8);
        draw->AddCircleFilled(ImVec2(x + width, cy), 2.0f, color, 8);
    } else if (boxType == 10) {
        float d = width * 0.15f;
        draw->AddLine(tl, ImVec2(tl.x + d, tl.y + d), color, th);
        draw->AddLine(tl, ImVec2(tl.x + d * 1.5f, tl.y), color, th);
        draw->AddLine(tl, ImVec2(tl.x, tl.y + d * 1.5f), color, th);
        draw->AddLine(tr, ImVec2(tr.x - d, tr.y + d), color, th);
        draw->AddLine(tr, ImVec2(tr.x - d * 1.5f, tr.y), color, th);
        draw->AddLine(tr, ImVec2(tr.x, tr.y + d * 1.5f), color, th);
        draw->AddLine(bl, ImVec2(bl.x + d, bl.y - d), color, th);
        draw->AddLine(bl, ImVec2(bl.x + d * 1.5f, bl.y), color, th);
        draw->AddLine(bl, ImVec2(bl.x, bl.y - d * 1.5f), color, th);
        draw->AddLine(br, ImVec2(br.x - d, br.y - d), color, th);
        draw->AddLine(br, ImVec2(br.x - d * 1.5f, br.y), color, th);
        draw->AddLine(br, ImVec2(br.x, br.y - d * 1.5f), color, th);
    }
}
