//
// Created by Luna Steed on 3/24/2024.
//

#include "mainmenu.h"
#include "game.h"

#include <QPushButton>
#include <iostream>


/**
 * @author Luna Steed
 * @time 3/24/2024
 * @brief Constructor for the main menu
 * @param parent The parent widget
 */

MainMenu::MainMenu(QWidget *parent) {
    grid = new QGridLayout();
    vbox = new QVBoxLayout();

    const int padding = 200;

    grid->setContentsMargins(padding, padding, padding, padding);
    this->setLayout(grid);
    grid->addLayout(vbox, 0, 0);

    if (std::filesystem::exists("assets/images/mainmenu.png")) {
        hasBackground = true;
        background = QPixmap("assets/images/mainmenu.png");
    }
    else{
        hasBackground = false;
        background = QPixmap();
    }

    title = new QLabel();
    title->setText("TANKS");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 50px; font-weight: bold; color: white;");

    vbox->addWidget(title);

    size_t id = 0;
    buttons = {"Select Level", "Quit"};

    for (auto& btn : buttons) {
        QPushButton* qbutton = new QPushButton(QString(btn.c_str()));

        vbox->addWidget(qbutton);

        connect(qbutton, &QPushButton::clicked, this, [this, id = id] { buttonClicked(id); });

        id++;
    }
}

void MainMenu::buttonClicked(int id) {
    switch (id) {
        case 0:
            Game::getInstance()->getWindow()->changeWidget(LEVEL_MENU_KEY);
            break;
        case 1:
            Game::destroyInstance();
            break;
        default:
            break;
    }
}

MainMenu::~MainMenu() {
    delete grid;
}

void MainMenu::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (hasBackground) {
        painter.drawPixmap(0, 0, this->width(), this->height(), background);
    }
    else{
        QWidget::paintEvent(event);
    }
}