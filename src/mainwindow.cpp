#include <QFileDialog>
#include <QSignalMapper>
#include <QLineEdit>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(0, "Output 1");
    ui->tabWidget->setTabText(1, "Output 2");
    ui->tabWidget->setTabText(2, "Output 3");
    connectButtons();
}

void MainWindow::connectButtons() {
    connect(ui->butBrowse, &QPushButton::clicked, [=](){ slotBrowse(ui->inputInputFolder); });
    connect(ui->butBrowseOutput1, &QPushButton::clicked, [=](){ slotBrowse(ui->inputOutputFolder1); });
    connect(ui->butBrowseOutput2, &QPushButton::clicked, [=](){ slotBrowse(ui->inputOutputFolder2); });
    connect(ui->butBrowseOutput3, &QPushButton::clicked, [=](){ slotBrowse(ui->inputOutputFolder3); });
    connect(ui->butBrowseWatermark1, &QPushButton::clicked, [=](){ slotBrowseWatermark(ui->inputWatermark1); });
    connect(ui->butBrowseWatermark2, &QPushButton::clicked, [=](){ slotBrowseWatermark(ui->inputWatermark2); });
    connect(ui->butBrowseWatermark3, &QPushButton::clicked, [=](){ slotBrowseWatermark(ui->inputWatermark3); });
}

void MainWindow::slotBrowse(QLineEdit *line) {
    QString dir = QFileDialog::getExistingDirectory(this, "Select input folder",
                                                    "",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    line->setText(dir);
}

void MainWindow::slotBrowseWatermark(QLineEdit *line) {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File",
                                                    "",
                                                    "Images (*.png *.xpm *.jpg)");
        line->setText(fileName);
}
MainWindow::~MainWindow()
{
    delete ui;
}
