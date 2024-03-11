#ifndef MAINMENU_H
#define MAINMENU_H

#include <QWidget>

namespace Ui {
class MainMenu : public QWidget {
    Q_OBJECT

public:
    MainMenu(QWidget* parent = nullptr);
    ~MainMenu();

    void setupUI(QWidget* widget); // or whatever the signature is
};
}
#endif // MAINMENU_H
