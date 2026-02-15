#pragma once
#include "shapes_simple.h"
#include "shapes_base.h"

struct Line : public Shape {
    double x1, y1, x2, y2;

    Line(double _x1, double _y1, double _x2, double _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {updateBoundingBox();} // recaclulate immediately
    void updateBoundingBox() {
        bbox_x = std::min(x1, x2) - 1;
        bbox_y = std::min(y1, y2) - 1;
        bbox_w = std::abs(x2 - x1) + 2;
        bbox_h = std::abs(y2 - y1) + 2;
    }
    
    void draw(QPainter& p) const override {
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(strokeColour, strokeWidth));
        p.drawLine(x1, y1, x2, y2);
    }

    void move(double dx, double dy) override {
        x1 += dx; y1 += dy;
        x2 += dx; y2 += dy;
        updateBoundingBox(); // Otherwise it doesn't let the orientation truly change anywhere.
    }

    // Handles at both endpoints
    int hitHandle(double px, double py) const override {
        if (std::abs(px - x1) <= HANDLE_SIZE/2 && std::abs(py - y1) <= HANDLE_SIZE/2) return 0;
        if (std::abs(px - x2) <= HANDLE_SIZE/2 && std::abs(py - y2) <= HANDLE_SIZE/2) return 1;
        return Shape::hitHandle(px, py);
    }
    void moveHandle(int handle, double dx, double dy) override {
        if (handle == 0) { x1 += dx; y1 += dy; }
        else if (handle == 1) { x2 += dx; y2 += dy; }
        else if (handle == HANDLE_LEFT) {
            if (x1 <= x2) x1 += dx; else x2 += dx;
        } else if (handle == HANDLE_RIGHT) {
            if (x1 >= x2) x1 += dx; else x2 += dx;
        } else if (handle == HANDLE_TOP) {
            if (y1 <= y2) y1 += dy; else y2 += dy;
        } else if (handle == HANDLE_BOTTOM) {
            if (y1 >= y2) y1 += dy; else y2 += dy;
        }
        updateBoundingBox();
    }
    void drawHandles(QPainter& p) const override {
        Shape::drawHandles(p);
        drawHandle(p, x1, y1);
        drawHandle(p, x2, y2);
    }
};
