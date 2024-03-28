//
// Created by lunah on 3/24/2024.
//

#ifndef TANKS_INGAMEMENU_H
#define TANKS_INGAMEMENU_H

#include <QtWidgets>

class InGameMenu : public QWidget {
    QPushButton *resumeButton;
    QPushButton *mainMenuButton;
    QPushButton *exitButton;
    QGridLayout *layout;

public:
    explicit InGameMenu(QWidget *parent = nullptr);
    ~InGameMenu() = default;
};


#endif //TANKS_INGAMEMENU_H
