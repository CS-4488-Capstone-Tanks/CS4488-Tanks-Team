//
// Created by lunah on 3/24/2024.
//

#ifndef TANKS_MAINMENU_H
#define TANKS_MAINMENU_H

#include <QtWidgets>

class MainMenu : public QWidget {
    Q_OBJECT

public:
    QLabel *title;
    QPushButton *playButton;
    QPushButton *optionsButton;
    QPushButton *exitButton;
    QGridLayout *layout;

    explicit MainMenu(QWidget *parent = nullptr);
};


#endif //TANKS_MAINMENU_H
