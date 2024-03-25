//
// Created by Luna Steed on 3/24/2024.
//

#include "mainmenu.h"

/**
 * @author Luna Steed
 * @time 3/24/2024
 * @brief Constructor for the main menu
 * @param parent The parent widget
 */
MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    // Create title
    title = new QLabel("Tanks", this);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 50px; font-weight: bold; color: white;");

    // Create buttons
    playButton = new QPushButton("Play", this);
    optionsButton = new QPushButton("Options", this);
    exitButton = new QPushButton("Exit", this);

    // Create layout
    layout = new QGridLayout(this);
    layout->addWidget(title, 0, 0, 1, 3);
    layout->addWidget(playButton, 1, 1);
    layout->addWidget(optionsButton, 2, 1);
    layout->addWidget(exitButton, 3, 1);

    // Set layout
    this->setLayout(layout);
}