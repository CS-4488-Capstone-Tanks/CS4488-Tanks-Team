//
// Created by lunah on 3/24/2024.
//

#include "optionsmenu.h"

/**
 * @author Luna Steed
 * @time 3/24/2024
 * @brief Constructor for the options menu
 * @param parent The parent widget
 */
OptionsMenu::OptionsMenu(QWidget *parent) {
    // Create title
    menuTitle = new QLabel("Options", parent);
    menuTitle->setAlignment(Qt::AlignCenter);
    menuTitle->setStyleSheet("font-size: 50px; font-weight: bold; color: white;");

    // Create buttons, sliders, selections, and labels
    returnButton = new QPushButton("Return to Main Menu", parent);
    volumeLabel = new QLabel("Volume", parent);
    volumeSlider = new QSlider(Qt::Horizontal, parent);
    resolutionLabel = new QLabel("Resolution", parent);
    resolutionComboBox = new QComboBox(parent);
    resolutions = new QStringList({"1920x1080", "1280x720", "800x600"});
    resolutionComboBox->addItems(*resolutions);
    fullscreenLabel = new QLabel("Fullscreen", parent);
    fullscreenCheckBox = new QCheckBox(parent);

    // Create layout
    layout = new QGridLayout(parent);
    layout->addWidget(menuTitle, 0, 0, 1, 3);
    layout->addWidget(returnButton, 1, 1);
    layout->addWidget(volumeLabel, 2, 0);
    layout->addWidget(volumeSlider, 2, 1);
    layout->addWidget(resolutionLabel, 3, 0);
    layout->addWidget(resolutionComboBox, 3, 1);
    layout->addWidget(fullscreenLabel, 4, 0);
    layout->addWidget(fullscreenCheckBox, 4, 1);

    // Set layout
    this->setLayout(layout);
}

/**
 * @author Luna Steed
 * @time 3/24/2024
 * @brief Destructor for the options menu
 */
OptionsMenu::~OptionsMenu() {
    delete returnButton;
    delete menuTitle;
    delete volumeLabel;
    delete volumeSlider;
    delete resolutionLabel;
    delete resolutionComboBox;
    delete resolutions;
    delete fullscreenLabel;
    delete fullscreenCheckBox;
    delete layout;
    delete this;
}