#include "levelmenu.h"
#include "gamestate.h"
#include "game.h"

#include <QPushButton>
#include <iostream>

LevelMenu::LevelMenu(QWidget* parent) {
    if (!std::filesystem::exists("assets/levels")) {
        std::cerr << "No levels found\n";
        return;
    }

    grid = new QGridLayout();
    this->setLayout(grid);

    size_t fileid = 0;
    for(const auto& entry : std::filesystem::directory_iterator("assets/levels")) {
        if (!entry.is_regular_file()) { continue; }

        auto path = entry.path();
        auto ext = path.extension();

        if (ext != "json") { continue; }

        levels.push_back(path);

        auto stem = path.stem().string();
        for(auto& chara : stem) { if (chara == '_') { chara = ' '; } } // replace _ with " "

        QPushButton* button = new QPushButton(QString(stem.c_str()));

        grid->addWidget(button, 0, fileid);

        connect(button, &QPushButton::clicked, this, [this, id = fileid] { buttonClicked(id); });
    }

}

void LevelMenu::buttonClicked(int id) {
    GameState::getInstance()->loadState(levels[id].string());
    Game::getInstance()->getWindow()->changeWidget(GAME_KEY);
}

LevelMenu::~LevelMenu() {
    delete grid;
}
