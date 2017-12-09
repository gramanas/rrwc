#include <QFileDialog>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QLineEdit>
#include <QDebug>
#include "mainwindow.hpp"
#include "outputmanager.hpp"
#include "ui_mainwindow.h"
#include "ui_outputtab.h"

//#include "exiv2/exif.hpp"
#include "exiv2/exiv2.hpp"

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
    connect(ui->actionSaveProfile, SIGNAL(triggered()), this, SLOT(actionSaveProfile()));
    connect(ui->actionLoadProfile, SIGNAL(triggered()), this, SLOT(actionLoadProfile()));

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
    delete m_outputTabs[index];
    m_outputTabs.remove(index);
    finalizeTabs();
}

void MainWindow::slotAddOutput() {
    OutputTab *outputTab = new OutputTab;
    int index = ui->tabWidget->addTab(outputTab, "Output");
    m_outputTabs.append(outputTab);
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
    if (!dir.isEmpty())
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

void MainWindow::actionSaveProfile() {
    QString filename = QFileDialog::getSaveFileName(this, "Save profile to",
                                                    QStandardPaths::locate(
                                                      QStandardPaths::HomeLocation,
                                                      "",
                                                      QStandardPaths::LocateDirectory) + QString("untitled.rrwcp"),
                                                    "Rrwc profile (*.rrwcp)");

    p_rrwc->outputManager()->generateOutputsFromTabs(m_outputTabs);
    p_rrwc->outputManager()->saveProfile(filename);
}

void MainWindow::actionLoadProfile() {
    QString filename = QFileDialog::getOpenFileName(this, "Load profile from",
                                                    QStandardPaths::locate(
                                                      QStandardPaths::HomeLocation,
                                                      "",
                                                      QStandardPaths::LocateDirectory) + QString("untitled.rrwcp"),
                                                    "Rrwc profile (*.rrwcp)");

    if (p_rrwc->outputManager()->loadProfile(filename)) {
        int oldSize = m_outputTabs.size();
        qDebug() << oldSize;
        for (int i = oldSize - 1; i >= 0; i--) {
            slotRemoveOutput(i);
        }
        m_outputTabs.clear();

        for (auto const output : p_rrwc->outputManager()->outputs()) {
            int i = output->index;
            slotAddOutput();
            m_outputTabs[i]->getUi()->inputOutputFolder->setText(output->folder);
            m_outputTabs[i]->getUi()->resize->setChecked(output->resize);
            m_outputTabs[i]->getUi()->inputLength->setValue(output->length);
            m_outputTabs[i]->getUi()->inputHeight->setValue(output->height);
            m_outputTabs[i]->getUi()->rename->setChecked(output->rename);
            m_outputTabs[i]->getUi()->inputRename->setText(output->renameText);
            m_outputTabs[i]->getUi()->inputCounterStart->setValue(output->counter.start);
            m_outputTabs[i]->getUi()->inputCounterStep->setValue(output->counter.step);
            m_outputTabs[i]->getUi()->inputCounterDigits->setValue(output->counter.digits);
            m_outputTabs[i]->getUi()->watermark->setChecked(output->watermark);
            m_outputTabs[i]->getUi()->inputWatermark->setText(output->watermarkText);
            m_outputTabs[i]->getUi()->inputOpacity->setValue(output->opacity);
            m_outputTabs[i]->getUi()->inputThreads->setValue(output->threads);
        }
        qDebug() << "Profile" << filename << "loaded";
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
