#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QLineEdit>
#include <QDebug>
#include "gui/mainwindow.hpp"
#include "output/outputmanager.hpp"
#include "globals.hpp"
#include "ui_mainwindow.h"
#include "ui_outputtab.h"

#include "exiv2/exif.hpp"
#include "exiv2/exiv2.hpp"

MainWindow::MainWindow(QWidget *parent, Rrwc *rrwc)
  : QMainWindow(parent),
    ui(new Ui::MainWindow),
    p_rrwc(rrwc) {
  ui->setupUi(this);
  setWindowTitle("Rrwc");
  connectButtons();
  slotAddOutput();

  connect(rrwc, SIGNAL(statusChanged(QString)), this, SLOT(slotStatusChanged(QString)));
  connect(rrwc, SIGNAL(done()), this, SLOT(onDone()));
  connect(rrwc, SIGNAL(started()), this, SLOT(onStarted()));

  connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(actionHelp()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actionAbout()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionSaveProfile, SIGNAL(triggered()), this, SLOT(actionSaveProfile()));
  connect(ui->actionLoadProfile, SIGNAL(triggered()), this, SLOT(actionLoadProfile()));
    }

void MainWindow::connectButtons() {
  connect(ui->butBrowse, &QPushButton::clicked, [=](){ slotBrowse(ui->inputInputFolder); });
  connect(ui->butAddOutput, &QPushButton::clicked, [=](){ slotAddOutput(); });
  connect(ui->tabWidget, SIGNAL (tabCloseRequested(int)), this, SLOT (slotRemoveOutput(int)));
  connect(ui->butGo, SIGNAL (clicked()), this, SLOT (slotGo()));
  connect(ui->butToggleLogOutputs, SIGNAL (clicked()), this, SLOT (slotToggleLogOutputs()));
}

void MainWindow::finalizeTabs() {
  // rename them according to checkboxes clicked
  for (int i = 0; i < m_outputTabs.size(); i++) {
    QString name = "";
    if (m_outputTabs[i]->getUi()->resize->isChecked())
      name += "Rs";
    if (m_outputTabs[i]->getUi()->rename->isChecked())
      name += "Rn";
    if (m_outputTabs[i]->getUi()->watermark->isChecked())
      name += "W";
    if (name == "") {
      ui->tabWidget->setTabText(i, QString::number(i + 1) + " - " + "Empty");
    } else {
      ui->tabWidget->setTabText(i, QString::number(i + 1) + " - " + name);
    }
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

// void MainWindow::slotWriteLog(QString log, QString str) {
//     if (log == LOG_ERROR) {
//         if (str == LOG_CLEAR) {
//             ui->outputErrorLog->setText("");
//             return;
//         }
//         ui->outputErrorLog->append(str);
//     } else if (log == LOG_PROGRESS) {
//         if (str == LOG_CLEAR) {
//             ui->outputProgressLog->setText("");
//             return;
//         }
//         ui->outputProgressLog->append(str);
//     }
// }

void MainWindow::slotToggleLogOutputs() {
  int index = ui->stackedWidget->currentIndex();
  int next = (index + 1) % 2;
  ui->stackedWidget->setCurrentIndex(next);

  if (next == 0) {
    ui->butToggleLogOutputs->setText(BUT_LOG);
    if (!m_isRunning) {
      ui->butAddOutput->setEnabled(true);
    }
  } else {
    ui->butToggleLogOutputs->setText(BUT_OUTPUTS);
    if (!m_isRunning) {
      ui->butAddOutput->setEnabled(false);
    }
  }
}

void MainWindow::slotAddOutput() {
  OutputTab *outputTab = new OutputTab;
  int index = ui->tabWidget->addTab(outputTab, "Output");

  connect(outputTab->getUi()->resize, SIGNAL(clicked()), this, SLOT(finalizeTabs()));
  connect(outputTab->getUi()->rename, SIGNAL(clicked()), this, SLOT(finalizeTabs()));
  connect(outputTab->getUi()->watermark, SIGNAL(clicked()), this, SLOT(finalizeTabs()));

  m_outputTabs.append(outputTab);
  finalizeTabs();
  ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::slotBrowse(QLineEdit *line) {
  QString dir = QFileDialog::
    getExistingDirectory(this, "Select input folder",
                         QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                "", QStandardPaths::LocateDirectory),
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  if (!dir.isEmpty())
    line->setText(dir);
}

void MainWindow::slotStatusChanged(QString status) {
  ui->progressBar->setFormat(status);
}

void MainWindow::slotGo() {
  p_rrwc->outputManager()->generateOutputsFromTabs(m_outputTabs);
  p_rrwc->go(ui->inputInputFolder->text(), ui->inputSortMode->currentText(), ui->inputThreads->value());
}

void MainWindow::enableLayout(bool t) {
  for (auto &it : m_outputTabs) {
    it->setEnabled(t);
  }

  ui->tabWidget->setTabsClosable(t);
  ui->menuBar->setEnabled(t);
  ui->butGo->setEnabled(t);
  // hack... remove
  if (ui->butToggleLogOutputs->text() != "Outputs")
    ui->butAddOutput->setEnabled(t);
  ui->inputInputFolder->setEnabled(t);
  ui->inputSortMode->setEnabled(t);
  ui->inputThreads->setEnabled(t);
  ui->butBrowse->setEnabled(t);
}

void MainWindow::onStarted() {
  enableLayout(false);
  m_isRunning = true;
  p_timer = new QTimer(this);
  p_timer->setInterval(500);
  connect(p_timer, &QTimer::timeout, this, [=]{
      int p = p_rrwc->outputManager()->progress();
      qDebug() << "prog:" << p;
      ui->progressBar->setValue(p);
    });
  p_timer->start();
}

void MainWindow::onDone() {
  enableLayout(true);
  if (ui->outputErrorLog->toPlainText() != "") {
    ui->outputProgressLog->append("Job's done, but something didn't go as planed...");
    ui->outputProgressLog->append("Check the error log below for details.");
    return;
  }
  ui->outputProgressLog->append("Job's done!");
  m_isRunning = false;
  ui->progressBar->setValue(100);
  p_timer->stop();
  delete p_timer;
}

void MainWindow::actionHelp() {
  QDesktopServices::openUrl(QUrl("https://github.com/gramanas/rrwc/wiki"));
}

void MainWindow::actionAbout() {
  QMessageBox msgBox(this);
  msgBox.setWindowTitle("About rrwc...");
  msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
  msgBox.setText("Version 1.0<br>Άντε και καλά ξεσκάρτ!<br><br>\
rrwc  Copyright (C) 2018  Anastasis Grammenos<br>\
This program comes with ABSOLUTELY NO WARRANTY.<br>\
This is free software, and you are welcome to redistribute it<br>\
under certain conditions.<br>\
See <a href='https://www.gnu.org/licenses/gpl-3.0.en.html'>GPLv3</a> for details.");
  msgBox.exec();
}

void MainWindow::actionSaveProfile() {
  QString filename = QFileDialog::
    getSaveFileName(this, "Save profile to",
                    QStandardPaths::locate(QStandardPaths::HomeLocation,
                                           "",
                                           QStandardPaths::LocateDirectory) + QString("untitled.rrwcp"),
                    "Rrwc profile (*.rrwcp)");

  if (!filename.isEmpty()) {
    p_rrwc->outputManager()->generateOutputsFromTabs(m_outputTabs);
    p_rrwc->outputManager()->saveProfile(filename);
    ui->outputProgressLog->append("Profile saved to " + filename);
  }
}

void MainWindow::actionLoadProfile() {
  QString filename = QFileDialog
    ::getOpenFileName(this, "Load profile from",
                      QStandardPaths::locate(QStandardPaths::HomeLocation,
                                             "",
                                             QStandardPaths::LocateDirectory) + QString("untitled.rrwcp"),
                      "Rrwc profile (*.rrwcp);;All files (*)");

  if (p_rrwc->outputManager()->loadProfile(filename)) {
    int oldSize = m_outputTabs.size();
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
      m_outputTabs[i]->getUi()->stripExifData->setChecked(output->stripMetadata);
    }
    //slotWriteLog(LOG_PROGRESS, "Profile " + filename + " loaded");
  }
  finalizeTabs();
}

MainWindow::~MainWindow() {
  delete ui;
}
