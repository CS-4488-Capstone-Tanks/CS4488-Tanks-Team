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
    else{
        hasBackground = false;
        background = QPixmap();
    }

    // Game Over text
    title = new QLabel("Game Over");
    title->setAlignment(Qt::AlignCenter);
    vbox->addWidget(title);

    // Comma Delimited Buttons. Easy to add new buttons, just add ,ButtonName to the string
    size_t id = 0;
    std::string button_string = "Restart,Quit,MainMenu";

    std::stringstream ss(button_string);

    for (int i; ss >> i;) {
        buttons.push_back(std::to_string(i));
        if (ss.peek() == ',')
            ss.ignore();
    }

    for (auto& btn : buttons) {
        QPushButton* qbutton = new QPushButton(QString(btn.c_str()));

        vbox->addWidget(qbutton);

        connect(qbutton, &QPushButton::clicked, this, [this, id = id] { buttonClicked(id); });

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
            Game::getInstance()->getWindow()->changeWidget(MAIN_MENU_KEY);
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
