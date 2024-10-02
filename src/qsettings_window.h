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
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with This program. If not, see <https://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#ifndef QSETTINGS_WINDOW_H
#define QSETTINGS_WINDOW_H

#include <memory>
#include <QDialog>
#include <QSettings>

namespace Ui {
class QSettings_Window;
}

class QSettings_Window : public QDialog
{
    Q_OBJECT

public:
    explicit QSettings_Window(QSettings* settings_ptr_main, QWidget *parent = nullptr);
    ~QSettings_Window();

private slots:

    void on_checkBox_output_audacity__stateChanged(int);

    void on_checkBox_output_summary_results_csv__stateChanged(int);

    void on_checkBox_output_summary_analyzed_files_csv__stateChanged(int);

    void on_button_ok__released();

    void on_button_default_settings__released();

private:
    Ui::QSettings_Window *ui;
    QSettings* settings_ptr;
    void resetSettings();
};

#endif // QSETTINGS_WINDOW_H
