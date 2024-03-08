#ifndef MENUABSTR_H
#define MENUABSTR_H

#include <QWidget>

class MenuAbstr : public QWidget
{
    Q_OBJECT
public:
    explicit MenuAbstr(QWidget *parent = nullptr);

signals:
};

#endif // MENUABSTR_H
