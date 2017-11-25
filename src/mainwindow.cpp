#include <QFileDialog>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QLineEdit>
#include <QDebug>
#include "mainwindow.hpp"
#include "outputmanager.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Rrwc *rrwc)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      p_rrwc(rrwc) {
    ui->setupUi(this);
    connectButtons();
    slotAddOutput();

    connect(rrwc, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)));
    connect(rrwc, SIGNAL(done(int)), ui->progressBar, SLOT(setValue(int)));
    connect(rrwc, SIGNAL(done(int)), this, SLOT(onDone()));
    connect(rrwc, SIGNAL(started()), this, SLOT(onStarted()));

    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(actionHelp()));
    
    ui->inputInputFolder->setText("/home/gramanas/Code/rrwc/tests/new");
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
    finalizeTabs();
}

void MainWindow::slotAddOutput() {
    OutputTab *output = new OutputTab;
    int index = ui->tabWidget->addTab(output, "Output");
    m_outputTabs.append(output);
    finalizeTabs();
    ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::slotBrowse(QLineEdit *line) {
    QString dir = QFileDialog::getExistingDirectory(this, "Select input folder",
                                                    QStandardPaths::locate(
                                                      QStandardPaths::HomeLocation,
                                                      "", QStandardPaths::LocateDirectory),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    line->setText(dir);
}

void MainWindow::slotGo() {
    p_rrwc->outputManager()->generateOutputsFromTabs(m_outputTabs);
    p_rrwc->outputManager()->print();
    p_rrwc->go(ui->inputInputFolder->text());
}

void MainWindow::enableLayout(bool t) {
    for (auto &it : m_outputTabs) {
        it->setEnabled(t);
    }

    ui->butGo->setEnabled(t);
    ui->butAddOutput->setEnabled(t);
    ui->inputInputFolder->setEnabled(t);
    ui->butBrowse->setEnabled(t);
}

void MainWindow::onStarted() {
    enableLayout(false);
}

void MainWindow::onDone() {
    enableLayout(true);
}

void MainWindow::actionHelp() {
    QDesktopServices::openUrl(QUrl("https://github.com/gramanas/rrwc/wiki"));
}

MainWindow::~MainWindow() {
    delete ui;
}
