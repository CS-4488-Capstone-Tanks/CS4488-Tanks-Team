//
// Created by lunah on 4/18/2024.
//

#include "gameover.h"
#include "gamestate.h"
#include "game.h"

#include <QPushButton>
#include <iostream>

GameOver::GameOver(QWidget* parent) {
    grid = new QGridLayout();
    vbox = new QVBoxLayout();

    const int padding = 200;

    grid->setContentsMargins(padding, padding, padding, padding);

    this->setLayout(grid);
    grid->addLayout(vbox, 0, 0);

    if (std::filesystem::exists("assets/images/gameover.png")) {
        hasBackground = true;
        background = QPixmap("assets/images/gameover.png");
    }
    // Game Over text
    QLabel* label = new QLabel("Game Over");
    label->setAlignment(Qt::AlignCenter);
    vbox->addWidget(label);

    // Comma Delimited Buttons. Easy to add new buttons, just add ,ButtonName to the string
    size_t id = 0;
    std::string button_string = "Restart,Quit,Main Menu";

    std::stringstream ss(button_string);

    for (int i; ss >> i;) {
        buttons.push_back(std::to_string(i));
        if (ss.peek() == ',')
            ss.ignore();
    }

    for (const auto& button : buttons) {
        QPushButton* button = new QPushButton(QString(button.c_str()));

        vbox->addWidget(button);

        connect(button, &QPushButton::clicked, this, [this, id = id] { buttonClicked(id); });

        id++;
    }
}

void GameOver::buttonClicked(int id) {
    switch (id) {
        case 0:
            Game::getInstance()->getWindow()->changeWidget(LEVEL_MENU_KEY);
            break;
        case 1:
            Game::destroyInstance();
            break;
        case 2:
            Game::getInstance()->getWindow()->changeWidget(MAINMENU_KEY);
            break;
        default:
            break;
    }
}

GameOver::~GameOver() {
    delete grid;
}

void GameOver::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (hasBackground) {
        painter.drawPixmap(0, 0, this->width(), this->height(), background);
    }
    else {
        QWidget::paintEvent(event);
    }
}