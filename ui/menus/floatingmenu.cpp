#include "floatingmenu.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "../../config/config.h"

FloatingMenu::FloatingMenu(QWidget* parent) : QWidget(parent) {
  setWindowFlags(Qt::Popup);  // auto-closes when user clicks elsewhere
  setAttribute(Qt::WA_DeleteOnClose, false);

  auto* layout = new QVBoxLayout(this);
  layout->setContentsMargins(config::kMenuMargin, config::kMenuMargin,
                             config::kMenuMargin, config::kMenuMargin);
  layout->setSpacing(config::kMenuSpacing);

  auto makeBtn = [&](const QString& label, void (FloatingMenu::*sig)()) {
    auto* btn = new QPushButton(label, this);
    btn->setFixedWidth(config::kMenuButtonWidth);
    connect(btn, &QPushButton::clicked, this, [this, sig]() {
      emit(this->*sig)();
      hide();
    });
    layout->addWidget(btn);
  };

  makeBtn("Copy", &FloatingMenu::CopyClicked);
  makeBtn("Cut", &FloatingMenu::CutClicked);
  makeBtn("Delete", &FloatingMenu::DeleteClicked);
  makeBtn("Paste", &FloatingMenu::PasteClicked);
}
