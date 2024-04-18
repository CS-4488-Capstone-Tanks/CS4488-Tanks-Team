//
// Created by lunah on 4/18/2024.
//

#ifndef CS4488_TANKS_TEAM_PAUSEMENU_H
#define CS4488_TANKS_TEAM_PAUSEMENU_H

#include <QWidget>
#include <QGridLayout>
#include <filesystem>

class PauseMenu: public QWidget {
    Q_OBJECT

    std::vector<std::string> buttons;
    QGridLayout* grid;
    QVBoxLayout* vbox;

public:
    explicit PauseMenu(QWidget* parent = nullptr);
    ~PauseMenu() override;

    void paintEvent(QPaintEvent *event) override;
private slots:
    void buttonClicked(int id);
};


#endif //CS4488_TANKS_TEAM_PAUSEMENU_H
