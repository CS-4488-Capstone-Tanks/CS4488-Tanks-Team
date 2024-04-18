//
// Created by lunah on 4/18/2024.
//

#ifndef CS4488_TANKS_TEAM_GAMEOVER_H
#define CS4488_TANKS_TEAM_GAMEOVER_H

#include <QWidget>
#include <QGridLayout>
#include <filesystem>

class GameOver : public QWidget {
    Q_OBJECT

    std::vector<std::string> buttons;
    QGridLayout* grid;
    QVBoxLayout* vbox;

public:
    explicit GameOver(QWidget* parent = nullptr);
    ~GameOver() override;

    void paintEvent(QPaintEvent *event) override;

private slots:
    void buttonClicked(int id);
};


#endif //CS4488_TANKS_TEAM_GAMEOVER_H
