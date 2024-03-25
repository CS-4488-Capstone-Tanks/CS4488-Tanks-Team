//
// Created by lunah on 3/24/2024.
// NOTE: NO FUNCTIONALITY ... (YET)
//

#ifndef TANKS_OPTIONSMENU_H
#define TANKS_OPTIONSMENU_H

#include <QtWidgets>

class OptionsMenu : public QWidget {
public:
    // Buttons, sliders, selections, and labels
    QPushButton *returnButton;
    QLabel *menuTitle;

    QLabel *volumeLabel;
    QSlider *volumeSlider;

    QLabel *resolutionLabel;
    QComboBox *resolutionComboBox;
    const QStringList *resolutions;

    QLabel *fullscreenLabel;
    QCheckBox *fullscreenCheckBox;

    QGridLayout *layout;

    // Constructor and destructor
    explicit OptionsMenu(QWidget *parent = nullptr);
    ~OptionsMenu() default;

};


#endif //TANKS_OPTIONSMENU_H
