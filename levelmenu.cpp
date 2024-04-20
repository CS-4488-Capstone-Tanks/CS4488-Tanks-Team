#include "levelmenu.h"
#include "scene.h"
#include "game.h"

#include <QPushButton>
#include <iostream>

LevelMenu::LevelMenu(QWidget* parent) {
    if (!std::filesystem::exists("assets/levels")) {
        std::cerr << "No levels found\n";
        return;
    }

    grid = new QGridLayout();
    vbox = new QVBoxLayout();

    const int padding = 200;

    grid->setContentsMargins(padding, padding, padding, padding);

    this->setLayout(grid);
    grid->addLayout(vbox, 0, 0);

    if (std::filesystem::exists("assets/images/levelmenu.png")) {
        hasBackground = true;
        background = QPixmap("assets/images/levelmenu.png");
    }

    size_t fileid = 0;
    for(const auto& entry : std::filesystem::directory_iterator("assets/levels")) {
        if (!entry.is_regular_file()) { continue; }

        auto path = entry.path();
        auto ext = path.extension().string();

        if (ext != ".json") { continue; }

        levels.push_back(path);

        auto stem = path.stem().string();
        for(auto& chara : stem) { if (chara == '_') { chara = ' '; } } // replace _ with " "

        QPushButton* button = new QPushButton(QString(stem.c_str()));

        //grid->addWidget(button, fileid, 0);
        vbox->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, id = fileid] { buttonClicked(id); });

        fileid++;
    }

}

void LevelMenu::buttonClicked(int id) {
    std::string levelname = levels[id].stem().string();
    Game::getInstance()->beginNewScene(levelname);
}

LevelMenu::~LevelMenu() {
    delete grid;
}

void LevelMenu::paintEvent(QPaintEvent* event) {
    if (hasBackground) {
        QPainter painter(this);
        painter.drawPixmap(0, 0, this->width(), this->height(), background);
    }
    else {
        QWidget::paintEvent(event);
    }
}
