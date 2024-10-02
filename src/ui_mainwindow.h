//------------------------------------------------------------------------------
//  Copyright 2016-2018 WavX inc. (www.wavx.ca)
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with This program.  If not, see <https://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <lineedit.h>
#include "settings.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *menu_about;
    QAction *action_reset_settings;
    QAction *action_edit_settings;
    QAction *action_start;
    QAction *action_stop;
    QAction *action_quit;
    QAction *action_about;
    QWidget *widget_central;
    QGridLayout *grid_layout;
    QProgressBar *progress_bar;
    QWidget *widget;
    QPushButton *button_exit, *button_start, *button_stop;
    QGroupBox *groupBox_input_f, *groupBox_output_f;
    QLabel *label_input_f, *label_output_f;
    QHBoxLayout *horizontalLayout_input_f;
    LineEdit *input_f;
    QLineEdit *output_f;
    QPushButton *button_input_f;
    QHBoxLayout *horizontalLayout_output_f;
    QPushButton *button_output_f;
    QMenuBar *menu_bar;
    QMenu *menu_menu;
    QMenu *menu_edit;
    QMenu *menu_help;

    QGroupBox *groupBox_settings;
    QHBoxLayout *horizontalLayout_settings;
    QCheckBox *checkBox_validation_audacity;
    QPushButton *button_default_settings, *button_settings_ok;
    QSpacerItem *horizontal_spacer_between_buttons_settings, *vertical_spacer_before_buttons_settings;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(626, 223);
        menu_about = new QAction(MainWindow);
        menu_about->setObjectName(QStringLiteral("menu_about"));
        menu_about->setMenuRole(QAction::ApplicationSpecificRole);
        action_reset_settings = new QAction(MainWindow);
        action_reset_settings->setObjectName(QStringLiteral("action_reset_settings"));
        action_edit_settings = new QAction(MainWindow);
        action_edit_settings->setObjectName(QStringLiteral("action_edit_settings"));
        action_start = new QAction(MainWindow);
        action_start->setObjectName(QStringLiteral("action_start"));
        action_stop =  new QAction(MainWindow);
        action_stop->setObjectName(QStringLiteral("action_stop"));
        action_quit = new QAction(MainWindow);
        action_quit->setObjectName(QStringLiteral("action_quit"));
        action_about = new QAction(MainWindow);
        action_about->setObjectName(QStringLiteral("action_about"));
        widget_central = new QWidget(MainWindow);
        widget_central->setObjectName(QStringLiteral("widget_central"));
        grid_layout = new QGridLayout(widget_central);
        grid_layout->setSpacing(6);
        grid_layout->setContentsMargins(11, 11, 11, 11);
        grid_layout->setObjectName(QStringLiteral("grid_layout"));
        grid_layout->setContentsMargins(-1, -1, 9, -1);

        widget = new QWidget(widget_central);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(100, 0));
        button_start = new QPushButton(widget);
        button_start->setObjectName(QStringLiteral("button_start"));
        button_start->setGeometry(QRect(10, 10, 91, 23));
        button_start->setDefault(true);
        button_stop = new QPushButton(widget);
        button_stop->setObjectName(QStringLiteral("button_stop"));
        button_stop->setGeometry(QRect(10, 40, 90, 23));
        button_stop->setMinimumSize(QSize(90, 23));
        button_stop->setMaximumSize(QSize(90, 23));
        button_exit = new QPushButton(widget);
        button_exit->setObjectName(QStringLiteral("button_exit"));
        button_exit->setGeometry(QRect(10, 70, 91, 23));

        grid_layout->addWidget(widget, 0, 2, 4, 1);

        groupBox_input_f = new QGroupBox(widget_central);
        groupBox_input_f->setObjectName(QStringLiteral("groupBox_input_f"));
        groupBox_input_f->setMinimumSize(QSize(0, 70));
        groupBox_input_f->setMaximumSize(QSize(16777215, 70));
        horizontalLayout_input_f = new QHBoxLayout(groupBox_input_f);
        horizontalLayout_input_f->setSpacing(6);
        horizontalLayout_input_f->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_input_f->setObjectName(QStringLiteral("horizontalLayout_input_f"));
        label_input_f = new QLabel(groupBox_input_f);
        label_input_f->setObjectName(QStringLiteral("label_input_f"));

        horizontalLayout_input_f->addWidget(label_input_f);

        input_f = new LineEdit(groupBox_input_f);
        input_f->setObjectName(QStringLiteral("input_f"));
        input_f->setFrame(true);
        input_f->setDragEnabled(true);
        input_f->setReadOnly(true);

        horizontalLayout_input_f->addWidget(input_f);

        button_input_f = new QPushButton(groupBox_input_f);
        button_input_f->setObjectName(QStringLiteral("button_input_f"));
        button_input_f->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_input_f->addWidget(button_input_f);


        grid_layout->addWidget(groupBox_input_f, 0, 0, 1, 2);

        groupBox_output_f = new QGroupBox(widget_central);
        groupBox_output_f->setObjectName(QStringLiteral("groupBox_output_f"));
        groupBox_output_f->setMinimumSize(QSize(0, 70));
        groupBox_output_f->setMaximumSize(QSize(16777215, 70));
        horizontalLayout_output_f = new QHBoxLayout(groupBox_output_f);
        horizontalLayout_output_f->setSpacing(6);
        horizontalLayout_output_f->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_output_f->setObjectName(QStringLiteral("horizontalLayout_output_f"));
        label_output_f = new QLabel(groupBox_output_f);
        label_output_f->setObjectName(QStringLiteral("label_output_f"));

        horizontalLayout_output_f->addWidget(label_output_f);

        output_f = new QLineEdit(groupBox_output_f);
        output_f->setObjectName(QStringLiteral("output_f"));
        output_f->setFrame(true);
        output_f->setDragEnabled(true);
        output_f->setReadOnly(true);

        horizontalLayout_output_f->addWidget(output_f);

        button_output_f = new QPushButton(groupBox_output_f);
        button_output_f->setObjectName(QStringLiteral("button_output_f"));

        horizontalLayout_output_f->addWidget(button_output_f);


        grid_layout->addWidget(groupBox_output_f, 2, 0, 1, 2);

        grid_layout->setColumnStretch(0, 5);
        grid_layout->setColumnStretch(1, 1);
        MainWindow->setCentralWidget(widget_central);
        menu_bar = new QMenuBar(MainWindow);
        menu_bar->setObjectName(QStringLiteral("menu_bar"));
        menu_bar->setGeometry(QRect(0, 0, 626, 19));
        menu_menu = new QMenu(menu_bar);
        menu_menu->setObjectName(QStringLiteral("menu_menu"));
        menu_edit = new QMenu(menu_bar);
        menu_edit->setObjectName(QStringLiteral("menu_edit"));
        menu_help = new QMenu(menu_bar);
        menu_help->setObjectName(QStringLiteral("menu_help"));
        MainWindow->setMenuBar(menu_bar);

        menu_bar->addAction(menu_menu->menuAction());
        menu_bar->addAction(menu_edit->menuAction());
        menu_bar->addAction(menu_help->menuAction());
        menu_menu->addAction(action_start);
        menu_menu->addAction(action_stop);
        menu_menu->addAction(action_quit);
        menu_edit->addAction(action_edit_settings);
        menu_help->addAction(action_about);

        // Progress bar
        progress_bar = new QProgressBar(widget_central);
        progress_bar->setObjectName(QStringLiteral("progress_bar"));
        progress_bar->setValue(0);
        progress_bar->setAlignment(Qt::AlignCenter);

        grid_layout->addWidget(progress_bar, 7, 0, 1, 3);
        // ---------------------------------------------------------

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CallSeeker", Q_NULLPTR));
        menu_menu->setTitle(QApplication::translate("MainWindow", "Menu", Q_NULLPTR));
        menu_edit->setTitle(QApplication::translate("MainWindow", "Edit", Q_NULLPTR));
        menu_help->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
        menu_about->setText(QApplication::translate("MainWindow", "About", Q_NULLPTR));

        action_reset_settings->setText(QApplication::translate("MainWindow", "Reset settings", Q_NULLPTR));
        action_edit_settings->setText(QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
        action_start->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        action_start->setShortcut(QApplication::translate("MainWindow", "F5", Q_NULLPTR));
        action_stop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        action_stop->setShortcut(QApplication::translate("MainWindow", "F6", Q_NULLPTR));
        action_quit->setText(QApplication::translate("MainWindow", "Quit", Q_NULLPTR));
        action_quit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", Q_NULLPTR));
        action_about->setText(QApplication::translate("MainWindow", "About", Q_NULLPTR));
        button_stop->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        button_exit->setText(QApplication::translate("MainWindow", "Exit", Q_NULLPTR));
        button_start->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        groupBox_input_f->setTitle(QApplication::translate("MainWindow", "Select the recordings to be analysed", Q_NULLPTR));
        label_input_f->setText(QApplication::translate("MainWindow", "Files:", Q_NULLPTR));
        button_input_f->setText(QApplication::translate("MainWindow", "Browse...", Q_NULLPTR));
        groupBox_output_f->setTitle(QApplication::translate("MainWindow", "Specify the folder where outputs will be saved", Q_NULLPTR));
        label_output_f->setText(QApplication::translate("MainWindow", "File:", Q_NULLPTR));
        button_output_f->setText(QApplication::translate("MainWindow", "Browse...", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
