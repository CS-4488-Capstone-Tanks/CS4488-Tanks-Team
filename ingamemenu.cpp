//
// Created by lunah on 3/24/2024.
//

#include "ingamemenu.h"

InGameMenu::InGameMenu(QWidget *parent) {
    resumeButton = new QPushButton("Resume", parent);
    mainMenuButton = new QPushButton("Main Menu", parent);
    exitButton = new QPushButton("Exit", parent);
    layout = new QGridLayout(parent);

    layout->addWidget(resumeButton, 0, 0);
    layout->addWidget(mainMenuButton, 1, 0);
    layout->addWidget(exitButton, 2, 0);
}

/**
 * @author Luna Steed
 * @time 3/24/2024
 * @brief Destructor for the in-game menu
 */
InGameMenu::~InGameMenu() {
    delete resumeButton;
    delete mainMenuButton;
    delete exitButton;
    delete layout;
    delete this;
}
