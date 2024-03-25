//
// Created by lunah on 3/24/2024.
//

#ifndef TANKS_INGAMEMENU_H
#define TANKS_INGAMEMENU_H

#include <QtWidgets>

class ingamemenu : public QWidget {
    QPushButton *resumeButton;
    QPushButton *mainMenuButton;
    QPushButton *exitButton;
    QGridLayout *layout;

public:
    explicit ingamemenu(QWidget *parent = nullptr);
    ~ingamemenu() override;
};


#endif //TANKS_INGAMEMENU_H
