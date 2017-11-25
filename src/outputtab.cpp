#include <QFileDialog>
#include <QStandardPaths>
#include <QLineEdit>
#include <QString>
#include <QString>

#include "outputtab.hpp"
#include "ui_outputtab.h"

OutputTab::OutputTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputTab)
{
    ui->setupUi(this);
    connectButtons();

    ui->inputOutputFolder->setText("/home/gramanas/Code/rrwc/tests/out");
}

OutputTab::~OutputTab()
{
    delete ui;
}

void OutputTab::connectButtons() {
    connect(ui->butBrowseOutput, &QPushButton::clicked, [=](){ slotBrowse(ui->inputOutputFolder); });
    connect(ui->butBrowseWatermark, &QPushButton::clicked, [=](){ slotBrowseWatermark(ui->inputWatermark); });
}

void OutputTab::slotBrowse(QLineEdit *line) {
     QString dir = QFileDialog::getExistingDirectory(this, "Select input folder",
                                                   QStandardPaths::locate(
                                                     QStandardPaths::HomeLocation,
                                                     "", QStandardPaths::LocateDirectory),
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);

     if (!dir.isEmpty())
         line->setText(dir);
}

void OutputTab::slotBrowseWatermark(QLineEdit *line) {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    "",
                                                    "Images (*.png *.xpm *.jpg)");
        line->setText(fileName);
}
