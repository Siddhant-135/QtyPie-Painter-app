#include "floatingmenu.h"

#include <QPushButton>
#include <QVBoxLayout>

FloatingMenu::FloatingMenu(QWidget* parent) : QWidget(parent) {
  setWindowFlags(Qt::Popup);  // auto-closes when user clicks elsewhere
  setAttribute(Qt::WA_DeleteOnClose, false);

  auto* layout = new QVBoxLayout(this);
  layout->setContentsMargins(4, 4, 4, 4);
  layout->setSpacing(2);

  auto makeBtn = [&](const QString& label, void (FloatingMenu::*sig)()) {
    auto* btn = new QPushButton(label, this);
    btn->setFixedWidth(80);
    connect(btn, &QPushButton::clicked, this, [this, sig]() {
      emit (this->*sig)();
      hide();
    });
    layout->addWidget(btn);
  };

  makeBtn("Copy",   &FloatingMenu::copyClicked);
  makeBtn("Cut",    &FloatingMenu::cutClicked);
  makeBtn("Delete", &FloatingMenu::deleteClicked);
  makeBtn("Paste",  &FloatingMenu::pasteClicked);
}
