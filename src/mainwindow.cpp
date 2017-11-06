#include <QFileDialog>
#include <QLineEdit>
#include <QDebug>
#include "mainwindow.hpp"
#include "outputmanager.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, OutputManager *outputManager)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {
    p_outputManager = outputManager;
    ui->setupUi(this);
    connectButtons();
    slotAddOutput();
}

void MainWindow::connectButtons() {
    connect(ui->butBrowse, &QPushButton::clicked, [=](){ slotBrowse(ui->inputInputFolder); });
    connect(ui->butAddOutput, &QPushButton::clicked, [=](){ slotAddOutput(); });
    connect(ui->tabWidget, SIGNAL (tabCloseRequested(int)), this, SLOT (slotRemoveOutput(int)));
    connect(ui->butGo, SIGNAL (clicked()), this, SLOT (slotGo()));
}

void MainWindow::finalizeTabs() {
    // rename them according to their index
    for (int i = 0; i < m_outputTabs.size(); i++) {
        ui->tabWidget->setTabText(i, "Output " + QString::number(i + 1));
    }

    // disable tab deletion it there is only one left
    if (m_outputTabs.size() == 1) {
        ui->tabWidget->setTabsClosable(false);
    } else {
        ui->tabWidget->setTabsClosable(true);
    }
}

void MainWindow::slotRemoveOutput(int index) {
    ui->tabWidget->removeTab(index);
    m_outputTabs.remove(index);
    qDebug() << "(-) # tabs: " << m_outputTabs.size();
    finalizeTabs();
}

void MainWindow::slotAddOutput() {
    OutputTab *output = new OutputTab;
    ui->tabWidget->addTab(output, "Output");
    m_outputTabs.append(output);
    qDebug() << "(+) # tabs: " << m_outputTabs.size();
    finalizeTabs();
}

void MainWindow::slotBrowse(QLineEdit *line) {
     // QString dir = QFileDialog::getExistingDirectory(this, "Select input folder",
     //                                                 "",
     //                                               QFileDialog::ShowDirsOnly
     //                                               | QFileDialog::DontResolveSymlinks);

    line->setText("lalala");
}

void MainWindow::slotGo() {
    p_outputManager->generateOutputsFromTabs(m_outputTabs);
    p_outputManager->print();
}

MainWindow::~MainWindow()
{
    delete ui;
}
