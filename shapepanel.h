#pragma once
#include <QWidget>
#include <memory>
#include <functional>
#include "shapes.h"

class ShapePanel : public QWidget {
    Q_OBJECT
public:
    explicit ShapePanel(QWidget* parent = nullptr);
signals:
    void request_shape(std::function<std::unique_ptr<Shape>()> factory);
    void request_clear();
};