#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

namespace Ui {
class MainMenu;
}

class MainMenu : public QWidget
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private:
    Ui::MainMenu *ui;
};
#endif // MAINMENU_H
