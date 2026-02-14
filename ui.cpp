#include "ui.h"

#include <QHBoxLayout>
#include <QPaintEvent>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
  setMinimumSize(400, 300);
  setAutoFillBackground(true);
}

void Canvas::showRectangle() {
  m_hasRectangle = true;
  update(); // ask Qt to repaint
}

void Canvas::clearRectangle() {
  m_hasRectangle = false;
  update();
}

void Canvas::paintEvent(QPaintEvent* event) {
  QWidget::paintEvent(event);

  if (!m_hasRectangle) return;

  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing, false);

  // Fixed-size rectangle (not dependent on window size).
  const int rectW = 180;
  const int rectH = 120;
  const int x = 30;
  const int y = 30;

  p.setPen(Qt::NoPen);
  p.setBrush(QColor(30, 144, 255)); // solid color (dodger blue)
  p.drawRect(x, y, rectW, rectH);
}

AppWindow::AppWindow(QWidget* parent) : QWidget(parent) {
  setWindowTitle("Qt Rectangle Demo");
  setMinimumSize(520, 420);

  m_canvas = new Canvas();
  auto* rectangleBtn = new QPushButton("Rectangle");
  auto* clearBtn = new QPushButton("Clear");

  QObject::connect(rectangleBtn, &QPushButton::clicked, [this]() {
    m_canvas->showRectangle();
  });
  QObject::connect(clearBtn, &QPushButton::clicked, [this]() {
    m_canvas->clearRectangle();
  });

  auto* buttons = new QHBoxLayout();
  buttons->addWidget(rectangleBtn);
  buttons->addWidget(clearBtn);
  buttons->addStretch();

  auto* layout = new QVBoxLayout(this);
  layout->addLayout(buttons);
  layout->addWidget(m_canvas, /*stretch=*/1);
}

