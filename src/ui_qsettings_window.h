#ifndef UI_QSETTINGS_WINDOW_H
#define UI_QSETTINGS_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include "settings.h"

QT_BEGIN_NAMESPACE

class Ui_QSettings_Window
{
public:
    QGridLayout *grid_layout_settings;
    QGroupBox *groupBox_output;
    QHBoxLayout *horizontalLayout_output;
    QPushButton *button_default_settings, *button_ok;
    QSpacerItem *horizontal_spacer_between_buttons, *vertical_spacer_before_buttons;
    QCheckBox *checkBox_output_audacity, *checkBox_output_summary_results_csv, *checkBox_output_summary_analyzed_files_csv;

    void setupUi(QDialog *QSettings_Window)
    {
        Settings default_settings;

        if (QSettings_Window->objectName().isEmpty())
            QSettings_Window->setObjectName(QStringLiteral("QSettings_Window"));
        QSettings_Window->setWindowModality(Qt::WindowModal);
        QSettings_Window->setModal(false);

        grid_layout_settings = new QGridLayout(QSettings_Window);
        grid_layout_settings->setSizeConstraint(QLayout::SetFixedSize);

        // Control output
        groupBox_output = new QGroupBox(QSettings_Window);
        horizontalLayout_output = new QHBoxLayout(groupBox_output);
        horizontalLayout_output->setAlignment(Qt::AlignHCenter);
        checkBox_output_audacity = new QCheckBox(QSettings_Window);
        checkBox_output_audacity->setLayoutDirection(Qt::RightToLeft);
        checkBox_output_audacity->setChecked(default_settings.output_audacity);
        checkBox_output_summary_results_csv = new QCheckBox(QSettings_Window);
        checkBox_output_summary_results_csv->setLayoutDirection(Qt::RightToLeft);
        checkBox_output_summary_results_csv->setChecked(default_settings.output_summary_results_csv);
        checkBox_output_summary_analyzed_files_csv = new QCheckBox(QSettings_Window);
        checkBox_output_summary_analyzed_files_csv->setLayoutDirection(Qt::RightToLeft);
        checkBox_output_summary_analyzed_files_csv->setChecked(default_settings.output_summary_analyzed_files_csv);

        horizontalLayout_output->addWidget(checkBox_output_summary_analyzed_files_csv);
        horizontalLayout_output->addWidget(checkBox_output_summary_results_csv);
        horizontalLayout_output->addWidget(checkBox_output_audacity);        
        // ---------------------------------------------------------

        // Buttons (default settings and OK)
        button_default_settings = new QPushButton(QSettings_Window);
        button_default_settings->setMinimumWidth(200);
        button_ok = new QPushButton(QSettings_Window);
        button_ok->setGeometry(QRect(10, 70, 91, 23));
        button_ok->setDefault(true);
        // ---------------------------------------------------------

        // Spacers
        horizontal_spacer_between_buttons = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);
        vertical_spacer_before_buttons = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Fixed);
        // ---------------------------------------------------------

        grid_layout_settings->addWidget(groupBox_output, 4, 1, 1, 5);
        grid_layout_settings->addItem(vertical_spacer_before_buttons, 5, 1, 1, 5);
        grid_layout_settings->addWidget(button_default_settings, 6, 2, 1, 1);
        grid_layout_settings->addItem(horizontal_spacer_between_buttons, 6, 3, 1, 1);
        grid_layout_settings->addWidget(button_ok, 6, 4, 1, 1);

        retranslateUi(QSettings_Window);

        // QMetaObject::connectSlotsByName(QSettings_Window);
    } // setupUi

    void retranslateUi(QDialog *QSettings_Window)
    {
        QSettings_Window->setWindowTitle(QApplication::translate("QSettings_Window", "Settings", Q_NULLPTR));

#ifndef QT_NO_TOOLTIP

        QString groupBox_output_tooltip = QApplication::translate(
                    "QSettings_Window",
                    "<html>Select the type of output files that CallSeeker should produce.</html>",
                    Q_NULLPTR);
        QString checkBox_output_audacity_tooltip = QApplication::translate(
                    "QSettings_Window",
                    "<html>Format detection times and predicted labels for Audacity.</html>",
                    Q_NULLPTR);
        QString checkBox_output_summary_results_csv_tooltip = QApplication::translate(
                    "QSettings_Window",
                    "<html>Produce a summary table with the results in csv format.</html>",
                    Q_NULLPTR);
        QString checkBox_output_summary_analyzed_files_csv_tooltip = QApplication::translate(
                    "QSettings_Window",
                    "<html>Produce a summary table listing the processed files and processing status.</html>",
                    Q_NULLPTR);

        groupBox_output->setToolTip(groupBox_output_tooltip);
        checkBox_output_audacity->setToolTip(checkBox_output_audacity_tooltip);
        checkBox_output_summary_results_csv->setToolTip(checkBox_output_summary_results_csv_tooltip);
        checkBox_output_summary_analyzed_files_csv->setToolTip(checkBox_output_summary_analyzed_files_csv_tooltip);

#endif // QT_NO_TOOLTIP

        groupBox_output->setTitle(QApplication::translate("QSettings_Window", "Type of output files", Q_NULLPTR));
        checkBox_output_audacity->setText(QApplication::translate("QSettings_Window", "Audacity (.txt)", Q_NULLPTR));
        checkBox_output_summary_results_csv->setText(QApplication::translate("QSettings_Window", "Analysis summary (.csv)", Q_NULLPTR));
        checkBox_output_summary_analyzed_files_csv->setText(QApplication::translate("QSettings_Window", "File processing status (.csv)", Q_NULLPTR));

        button_default_settings->setText(QApplication::translate("QSettings_Window", "Default Settings", Q_NULLPTR));
        button_ok->setText(QApplication::translate("QSettings_Window", "OK", Q_NULLPTR));

    } // retranslateUi

};

namespace Ui {
    class QSettings_Window: public Ui_QSettings_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QSETTINGS_WINDOW_H
