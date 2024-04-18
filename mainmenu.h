//
// Created by lunah on 3/24/2024.
//

#ifndef TANKS_MAINMENU_H
#define TANKS_MAINMENU_H

#include <QWidget>
#include <QGridLayout>
#include <filesystem>

class MainMenu : public QWidget {
    Q_OBJECT

    QLabel* title;
    std::vector<std::string> buttons;
    QGridLayout* grid;
    QVBoxLayout* vbox;
public:
    explicit MainMenu(QWidget* parent = nullptr);
    ~MainMenu() override;

    void paintEvent(QPaintEvent *event) override;

private slots:
    void buttonClicked(int id);
};


#endif //TANKS_MAINMENU_H
