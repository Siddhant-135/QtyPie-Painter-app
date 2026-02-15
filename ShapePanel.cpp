#include "shapepanel.h"
#include <QHBoxLayout>
#include <QPushButton>

ShapePanel::ShapePanel(QWidget* parent) : QWidget(parent) {
    auto* layout = new QHBoxLayout(this);
    const std::vector<std::pair<QString, std::function<std::unique_ptr<Shape>()>>> shapeTypes = {  // GOated Idea -> Instead of doing declare button - name - add signal for each thing, we just define in one place then use this "factory feature"
        {"Rectangle", [] { return std::make_unique<Rectangle>(); }}, {"Square",    [] { return std::make_unique<Square>();    }}, 
        {"Circle",    [] { return std::make_unique<Circle>();    }}, {"Line",      [] { return std::make_unique<Line>();      }}
    };

    for (const auto& [name, factory] : shapeTypes) {
        auto* btn = new QPushButton(name, this);
        connect(btn, &QPushButton::clicked, this, [this, f = factory]() {
            emit request_shape(f); 
        });
        layout->addWidget(btn);
    }

    auto* clearBtn = new QPushButton("Clear", this);
    connect(clearBtn, &QPushButton::clicked, this, &ShapePanel::request_clear);
    layout->addWidget(clearBtn);
    layout->addStretch(); // Good UI thing which spreads stuff out
}