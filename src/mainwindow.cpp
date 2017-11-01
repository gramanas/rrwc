#include <QFileDialog>
#include <QLineEdit>
#include <QDebug>
#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectButtons();
    slotAddOutput();
}

void MainWindow::connectButtons() {
    connect(ui->butBrowse, &QPushButton::clicked, [=](){ slotBrowse(ui->inputInputFolder); });
    connect(ui->butAddOutput, &QPushButton::clicked, [=](){ slotAddOutput(); });
    connect(ui->tabWidget, SIGNAL (tabCloseRequested(int)), this, SLOT (slotRemoveOutput(int)));
}

void MainWindow::finalizeTabs() {
    // rename them according to their index
    for (int i = 0; i < m_outputs.size(); i++) {
        ui->tabWidget->setTabText(i, "Output " + QString::number(i + 1));
    }

    // disable tab deletion it there is only one left
    if (m_outputs.size() == 1) {
        ui->tabWidget->setTabsClosable(false);
    } else {
        ui->tabWidget->setTabsClosable(true);
    }
}

void MainWindow::slotRemoveOutput(int index) {
    ui->tabWidget->removeTab(index);
    m_outputs.remove(index);
    qDebug() << "(-) # tabs: " << m_outputs.size();
    finalizeTabs();
}

void MainWindow::slotAddOutput() {
    OutputTab *output = new OutputTab;
    ui->tabWidget->addTab(output, "Output");
    m_outputs.append(output);
    qDebug() << "(+) # tabs: " << m_outputs.size();
    finalizeTabs();
}

void MainWindow::slotBrowse(QLineEdit *line) {
     QString dir = QFileDialog::getExistingDirectory(this, "Select input folder",
                                                   "",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);

    line->setText(dir);
}

MainWindow::~MainWindow()
{
    delete ui;
}
