#ifndef TANKS_LEVELMENU_H
#define TANKS_LEVELMENU_H

#include <QWidget>
#include <QGridLayout>
#include <filesystem>

class LevelMenu : public QWidget {
    Q_OBJECT

    std::vector<std::filesystem::path> levels;
    QGridLayout* grid;
    QVBoxLayout* vbox;
public:
    explicit LevelMenu(QWidget* parent = nullptr);
    ~LevelMenu() override;

private slots:
    void buttonClicked(int id);
};

#endif //TANKS_LEVELMENU_H
