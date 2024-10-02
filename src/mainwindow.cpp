//------------------------------------------------------------------------------
//  Copyright 2012 Chris Scott (fbscds@gmail.com)
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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <QDragEnterEvent>
#include <QFileDialog>
#include <QMimeData>
#include "lineedit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "about_window.h"
#include "qsettings_window.h"
#include "settings.h"
#include "worker.h"
#include "tensorflow/core/protobuf/config.pb.h"
#include "tensorflow/cc/saved_model/tag_constants.h"
#include "tensorflow/core/public/session.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    readQSettings();

    ui->setupUi(this);
    ui->output_f->setText(app_output_dir_path.absolutePath());

    connect(ui->action_start, &QAction::triggered, this, &MainWindow::run);
    connect(ui->action_stop, &QAction::triggered, this, &MainWindow::stop_requested);
    connect(ui->action_quit, &QAction::triggered, this, &MainWindow::quit);
    connect(ui->action_edit_settings, &QAction::triggered, this, &MainWindow::settings);
    connect(ui->action_about, &QAction::triggered, this, &MainWindow::about);

    Worker *worker = new Worker;

    connect(this, SIGNAL(stop_requested()), worker, SLOT(received_stop_signal()), Qt::DirectConnection);
    connect(worker, SIGNAL(finished()), this, SLOT(analysis_finished()));
    connect(worker, SIGNAL(read_file(float)), this, SLOT(pg_bar_value_needs_update(float)));
    connect(worker, SIGNAL(read_file(QString)), this, SLOT(pg_bar_message_needs_update(QString)));
    connect(worker, SIGNAL(process_file(float)), this, SLOT(pg_bar_value_needs_update(float)));
    connect(worker, SIGNAL(process_file(QString)), this, SLOT(pg_bar_message_needs_update(QString)));

    worker->moveToThread(&thread);

    qRegisterMetaType<QDir>("QDir");

    connect(&thread, &QThread::started, worker, &Worker::loadModel);
    connect(this, SIGNAL(run_impl(QStringList, QSettings*, QDir)),
            worker, SLOT(run_impl(QStringList, QSettings*, QDir)));
    connect(&thread, &QThread::finished, worker, &QObject::deleteLater);
    thread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    thread.quit();
    thread.wait();
}

void MainWindow::on_button_input_f_released()
{
    QFileDialog input_files_dialog;
    input_files_dialog.setFileMode(QFileDialog::ExistingFiles);
    input_files_dialog.setViewMode(QFileDialog::Detail);
    input_files_dialog.setNameFilters(QStringList()<<"*.wav");
    input_files_dialog.setWindowTitle(tr("Select a recording"));
    input_files_dialog.exec();

    if (!input_files_dialog.selectedFiles().isEmpty() && input_files_dialog.result() == QDialog::Accepted)
    {
        ui->input_f->filepaths = input_files_dialog.selectedFiles();
        // Update source files
        ui->input_f->setText(ui->input_f->filepaths.join(", "));
    }
}

void MainWindow::on_button_output_f_released()
{
    QFileDialog output_dir_dialog;
    output_dir_dialog.setDirectory(ui->output_f->text());
    output_dir_dialog.setFileMode(QFileDialog::Directory);
    output_dir_dialog.setOption(QFileDialog::ShowDirsOnly, true);
    output_dir_dialog.setWindowTitle(tr("Select a folder"));
    output_dir_dialog.exec();

    if (!output_dir_dialog.selectedFiles().isEmpty() && output_dir_dialog.result() == QDialog::Accepted)
    {
        QDir output_dir_path( output_dir_dialog.selectedFiles().value(0) );

        if (output_dir_path.exists())
        {
            app_output_dir_path = output_dir_path;
            ui->output_f->setText(app_output_dir_path.absolutePath());
        }
    }
}

void MainWindow::on_button_start_released()
{
    run();
}

void MainWindow::on_button_stop_released()
{
    emit stop_requested();
}

void MainWindow::on_button_exit_released()
{
    emit stop_requested();
    quit();
}

void MainWindow::settings()
{
    QSettings_Window qsettings_window(&app_settings);
    qsettings_window.exec();
}

void MainWindow::about()
{
    About_Window about_window;
    about_window.setModal(true);
    about_window.exec();
}

void MainWindow::readQSettings() {

    Settings default_settings;

    app_settings.setValue("output_audacity", app_settings.value("output_audacity", default_settings.output_audacity));
    app_settings.setValue("output_summary_results_csv", app_settings.value("output_summary_results_csv", default_settings.output_summary_results_csv));
    app_settings.setValue("output_summary_analyzed_files_csv", app_settings.value("output_summary_analyzed_files_csv", default_settings.output_summary_analyzed_files_csv));
}

void MainWindow::run()
{
    if (ui->input_f->filepaths.size() > 0 && app_output_dir_path.exists())
    {
        ui->progress_bar->setValue(0);
        ui->button_start->setDisabled(true);
        ui->action_start->setDisabled(true);

        emit run_impl(ui->input_f->filepaths, &app_settings, app_output_dir_path);
    }
}

void MainWindow::update_pg_bar_message (QString str)
{
    ui->progress_bar->setFormat(str);
}

void MainWindow::update_pg_bar_value(float value)
{
    ui->progress_bar->setValue(int(value));
}

void MainWindow::quit()
{
    QApplication::closeAllWindows();
    QApplication::quit();
}

