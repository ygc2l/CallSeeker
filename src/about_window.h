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

#ifndef ABOUT_WINDOW_H
#define ABOUT_WINDOW_H

#include <QDialog>

namespace Ui {
class About_Window;
}

class About_Window : public QDialog
{
    Q_OBJECT

public:
    explicit About_Window(QWidget *parent = nullptr);
    ~About_Window();

private slots:

    void on_closeButton_released();

private:
    Ui::About_Window *ui;
};

#endif // ABOUT_WINDOW_H
