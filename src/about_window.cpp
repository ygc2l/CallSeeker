//------------------------------------------------------------------------------
//  Copyright 2016-2017 WavX inc. (www.wavx.ca)
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

#include "about_window.h"
#include "ui_about_window.h"

About_Window::About_Window(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About_Window)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

About_Window::~About_Window()
{
    delete ui;
}

void About_Window::on_closeButton_released()
{
    this->close();
}
