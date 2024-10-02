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
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with This program. If not, see <https://www.gnu.org/licenses/>.
//------------------------------------------------------------------------------

#include "settings.h"
#include "qsettings_window.h"
#include "ui_qsettings_window.h"

QSettings_Window::QSettings_Window(QSettings* settings_ptr_main, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QSettings_Window)
{
  settings_ptr = settings_ptr_main;

  Qt::WindowFlags flags = this->windowFlags();
  flags = flags & ~Qt::WindowContextHelpButtonHint;
  this->setWindowFlags(flags);

  ui->setupUi(this);

  connect(ui->checkBox_output_audacity, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_output_audacity__stateChanged(int)));
  connect(ui->checkBox_output_summary_results_csv, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_output_summary_results_csv__stateChanged(int)));
  connect(ui->checkBox_output_summary_analyzed_files_csv, SIGNAL(stateChanged(int)), this, SLOT(on_checkBox_output_summary_analyzed_files_csv__stateChanged(int)));
  connect(ui->button_ok, SIGNAL(released()), this, SLOT(on_button_ok__released()));
  connect(ui->button_default_settings, SIGNAL(released()), this, SLOT(on_button_default_settings__released()));

  ui->checkBox_output_audacity->setChecked(settings_ptr->value("output_audacity").toBool());
  ui->checkBox_output_summary_results_csv->setChecked(settings_ptr->value("output_summary_results_csv").toBool());
  ui->checkBox_output_summary_analyzed_files_csv->setChecked(settings_ptr->value("output_summary_analyzed_files_csv").toBool());
}

QSettings_Window::~QSettings_Window()
{
    delete ui;
}

void QSettings_Window::on_checkBox_output_audacity__stateChanged(int)
{
    settings_ptr->setValue("output_audacity", ui->checkBox_output_audacity->isChecked());
}

void QSettings_Window::on_checkBox_output_summary_results_csv__stateChanged(int)
{
    settings_ptr->setValue("output_summary_results_csv", ui->checkBox_output_summary_results_csv->isChecked());
}

void QSettings_Window::on_checkBox_output_summary_analyzed_files_csv__stateChanged(int)
{
    settings_ptr->setValue("output_summary_analyzed_files_csv", ui->checkBox_output_summary_analyzed_files_csv->isChecked());
}

void QSettings_Window::on_button_default_settings__released()
{
    resetSettings();
}

void QSettings_Window::on_button_ok__released()
{
    close();
}

void QSettings_Window::resetSettings()
{
    Settings defaultSettings;

    settings_ptr->setValue("output_audacity", defaultSettings.output_audacity);
    settings_ptr->setValue("output_summary_results_csv", defaultSettings.output_summary_results_csv);
    settings_ptr->setValue("output_summary_analyzed_files_csv", defaultSettings.output_summary_analyzed_files_csv);

    ui->checkBox_output_audacity->setChecked(defaultSettings.output_audacity);
    ui->checkBox_output_summary_results_csv->setChecked(defaultSettings.output_summary_results_csv);
    ui->checkBox_output_summary_analyzed_files_csv->setChecked(defaultSettings.output_summary_analyzed_files_csv);
}
