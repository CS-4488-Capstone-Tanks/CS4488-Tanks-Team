//
// Created by lunah on 4/18/2024.
//

#include "pausemenu.h"
#include "gamestate.h"
#include "game.h"

#include <QPushButton>
#include <iostream>

PauseMenu::PauseMenu(QWidget *parent) {
    grid = new QGridLayout();
    vbox = new QVBoxLayout();

    const int padding = 200;

    grid->setContentsMargins(padding, padding, padding, padding);
    if (std::filesystem::exists("assets/images/pausemenu.png")) {
        hasBackground = true;
        background = QPixmap("assets/images/pausemenu.png");
    }
    else{
        hasBackground = false;
        background = QPixmap();
    }

    size_t id = 0;
    std::string button_string = "Resume,Quit,Main Menu";

    std::stringstream ss(button_string);

    for (int i; ss >> i;) {
        buttons.push_back(std::to_string(i));
        if (ss.peek() == ',')
            ss.ignore();
    }

    for (const auto& btn : buttons) {
        QPushButton* qbutton = new QPushButton(QString(btn.c_str()));

        vbox->addWidget(qbutton);

        connect(qbutton, &QPushButton::clicked, this, [this, id = id] { buttonClicked(id); });

        id++;
    }
}

void PauseMenu::buttonClicked(int id) {
    switch (id) {
        case 0:
            Game::getInstance()->getWindow()->changeWidget(GAME_KEY);
            break;
        case 1:
            Game::destroyInstance();
            break;
        case 2:
            Game::getInstance()->getWindow()->changeWidget(MAIN_MENU_KEY);
            break;
        default:
            break;
    }
}

PauseMenu::~PauseMenu() {
    delete grid;
}

void PauseMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (hasBackground) {
        painter.drawPixmap(0, 0, this->width(), this->height(), background);
    }
    else {
        QWidget::paintEvent(event);
    }
}