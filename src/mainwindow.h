//------------------------------------------------------------------------------
//  Copyright 2017-2018 WavX inc. (www.wavx.ca)
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <QDir>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include "ui_mainwindow.h"
#include "utils.h"
#include "tensorflow/cc/saved_model/loader.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QThread thread;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSettings app_settings;
    QDir app_output_dir_path;

signals:
    void run_impl(QStringList filepaths, QSettings* settings_ptr, QDir output_dir_path);
    void stop_requested();

public slots:
    void analysis_finished()
    {
        update_pg_bar_message("Analysis complete!");
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::NoIcon);
        msgBox.setText("Analysis complete!");
        msgBox.exec();

        ui->button_start->setEnabled(true);
        ui->action_start->setEnabled(true);
    }

    void pg_bar_value_needs_update (float v)
    {
        update_pg_bar_value(v);
    }

    void pg_bar_message_needs_update (QString s)
    {
        update_pg_bar_message(s);
    }

private slots:
    void settings();
    void about();
    void quit();

    void on_button_input_f_released();
    void on_button_output_f_released();
    void on_button_start_released();
    void on_button_stop_released();
    void on_button_exit_released();

private:
    Ui::MainWindow *ui;    

    tensorflow::SavedModelBundle model;

    void readQSettings();
    void run();
    void update_pg_bar_message(QString str);
    void update_pg_bar_value(float value);
};

#endif // MAINWINDOW_H
