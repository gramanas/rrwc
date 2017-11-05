#include <QFileDialog>
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
                                                   "",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);

    line->setText(dir);
}

void OutputTab::slotBrowseWatermark(QLineEdit *line) {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    "",
                                                    "Images (*.png *.xpm *.jpg)");
        line->setText(fileName);
}