#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <QApplication>
#include <QObject>
#include <QLabel>
#include <QWindow>
#include <QWidget>

class MenuManager : public QObject {
Q_OBJECT
public:
    MenuManager();
    void receive_signal();

protected:
    QWidget* active_menu;
    std::vector<QWidget*> QW_Vec;

    QWidget Main_Menu;
    QWidget In_Game_Menu;
    QWidget Options_Menu;
    QWidget Multiplayer_Menu;
    QWidget Host_Menu;
    QWidget Join_IP_Menu;

    Q_SLOT QWidget* fetch_menu(int key);

    friend class GameWindow;
};


#endif // MENUMANAGER_H
