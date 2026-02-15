#pragma once
#include <QWidget>

class MyCanvas; // Forward declaration
class ColourRibbon;
class ShapePanel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;

class MainWidget : public QWidget {
    Q_OBJECT
public:
    explicit MainWidget(QWidget* parent = nullptr);
private:
    MyCanvas* mycanvas = nullptr;
    ColourRibbon* colourPanel  = nullptr;
    ShapePanel* shapePanel;
};