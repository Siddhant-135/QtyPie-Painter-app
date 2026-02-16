#pragma once
#include "shapes_simple.h"
#include "shapes_base.h"

struct Rectangle : public Shape {
  double x = bbox_x; double y = bbox_y; double w = bbox_w; double h = bbox_h;
    void draw_obj(QPainter& p) const override {
    p.setPen(QPen(strokeColour, strokeWidth));
    p.setBrush(fillColour);
    p.drawRect(x, y, w, h);
  }
    void move_obj(double dx, double dy) override {
        Shape::move_obj(dx, dy);
    x += dx; y += dy;
  }
  void onBBoxChanged() override {
    x = bbox_x;
    y = bbox_y;
    w = bbox_w;
    h = bbox_h;
  }
};

struct RoundedRectangle : public Rectangle {
    double cornerRadius = 0;

    void draw_obj(QPainter& p) const override {
        p.setPen(QPen(strokeColour, strokeWidth));
        p.setBrush(fillColour);
        p.drawRoundedRect(x, y, w, h, cornerRadius, cornerRadius);
    }
    void updateRounding(double sliderPos) {
        double shortSide = std::min(w, h);
        cornerRadius = shortSide * std::min(0.5, sliderPos);
    }

    // Handle: slides along top edge to control corner radius
    int hitHandle(double px, double py) const override {
        double hx = x + cornerRadius, hy = y;
        if (std::abs(px - hx) <= HANDLE_SIZE/2 && std::abs(py - hy) <= HANDLE_SIZE/2) return 0;
        return Shape::hitHandle(px, py);
    }
    void moveHandle(int handle, double dx, double dy) override {
        if (handle == 0) {
            double maxR = std::min(w, h) / 2.0;
            cornerRadius = std::max(0.0, std::min(cornerRadius + dx, maxR));
        } else {
            Shape::moveHandle(handle, dx, dy);
            cornerRadius = std::max(0.0, std::min(cornerRadius, std::min(w, h) / 2.0));
        }
    }
    void drawHandles(QPainter& p) const override {
        Shape::drawHandles(p);
        drawHandle(p, x + cornerRadius, y);
    }
    void onBBoxChanged() override {
        x = bbox_x;
        y = bbox_y;
        w = bbox_w;
        h = bbox_h;
        cornerRadius = std::max(0.0, std::min(cornerRadius, std::min(w, h) / 2.0));
    }
};
