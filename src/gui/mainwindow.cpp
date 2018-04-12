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

MainWindow::MainWindow(QWidget *parent, Rrwc *rrwc,
                       const CliOptions &opt)
  : QMainWindow(parent),
    ui(new Ui::MainWindow),
    p_rrwc(rrwc)
{
  ui->setupUi(this);

  // set logs font
  QFont f("unexistend");
  f.setStyleHint(QFont::Monospace);
  ui->outputErrorLog->setFont(f);
  ui->outputProgressLog->setFont(f);
  initializeLogging();

  // set input folder
  ui->inputInputFolder->setText(opt.input);

  // set sort mode
  (opt.sortExif) ?
    ui->inputSortMode->setCurrentText(SORT_EXIF) :
    ui->inputSortMode->setCurrentText(SORT_FILENAME);

  // set threads
  ui->inputThreads->setValue(opt.threads);

  setWindowTitle("Rrwc");


  connectButtons();
  connectActions();

  if (!opt.profile.isEmpty()) {
    if (!loadProfile(opt.profile)) {
      p_rrwc->logger()->err(ERR_WRONG_PROFILE + "\n" + opt.profile);
      slotAddOutput();
    }
  }
  else {
    slotAddOutput();
  }

  connect(rrwc, SIGNAL(done()), this, SLOT(onDone()));
  connect(rrwc, SIGNAL(started()), this, SLOT(onStarted()));

}

void MainWindow::initializeLogging() {
  p_logTimer = new QTimer(this);
  p_logTimer->setInterval(200);
  connect(p_logTimer, &QTimer::timeout, this, [=]{
      QString log = p_rrwc->logger()->flushLog();
      if (!log.isEmpty()) {
        ui->outputProgressLog->append(log);
      }
      QString err = p_rrwc->logger()->flushErr();
      if (!err.isEmpty()) {
        ui->outputErrorLog->append(err);
      }
      QString status = p_rrwc->logger()->flushStatus();
      if (!status.isEmpty() && status != ui->progressBar->format()) {
        ui->progressBar->setFormat(status);
      }
      uint prog = p_rrwc->logger()->getProgress();
      if (prog != ui->progressBar->value()) {
        ui->progressBar->setValue(prog);
      }
    });
  p_logTimer->start();
}

void MainWindow::connectButtons() {
  connect(ui->butBrowse, &QPushButton::clicked, [=](){ slotBrowse(ui->inputInputFolder); });
  connect(ui->butAddOutput, &QPushButton::clicked, [=](){ slotAddOutput(); });
  connect(ui->tabWidget, SIGNAL (tabCloseRequested(int)), this, SLOT (slotRemoveOutput(int)));
  connect(ui->butGo, SIGNAL (clicked()), this, SLOT (slotGo()));
  connect(ui->butToggleLogOutputs, SIGNAL (clicked()), this, SLOT (slotToggleLogOutputs()));
}

void MainWindow::connectActions() {
  connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(actionHelp()));
  connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actionAbout()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionSaveProfile, SIGNAL(triggered()), this, SLOT(actionSaveProfile()));
  connect(ui->actionLoadProfile, SIGNAL(triggered()), this, SLOT(actionLoadProfile()));
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
    if (m_outputTabs[i]->getUi()->comment->isChecked())
      name += "C";
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
  connect(outputTab->getUi()->comment, SIGNAL(clicked()), this, SLOT(finalizeTabs()));

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
}

void MainWindow::onDone() {
  enableLayout(true);
  p_rrwc->logger()->done();
  p_rrwc->logger()->log("Job's done!");
  m_isRunning = false;
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
    p_rrwc->logger()->log("Profile saved to " + filename);
  }
}

bool MainWindow::loadProfile(const QString &path) {
  if (p_rrwc->outputManager()->loadProfile(path)) {
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
      m_outputTabs[i]->getUi()->stripMetadata->setChecked(output->stripMetadata);
      m_outputTabs[i]->getUi()->comment->setChecked(output->comment);
      m_outputTabs[i]->getUi()->inputComment->setText(output->commentText);
    }
    p_rrwc->logger()->log("Profile " + path + " loaded");
    finalizeTabs();
    return true;
  }
  return false;
}

void MainWindow::actionLoadProfile() {
  QString filename = QFileDialog
    ::getOpenFileName(this, "Load profile from",
                      QStandardPaths::locate(QStandardPaths::HomeLocation,
                                             "",
                                             QStandardPaths::LocateDirectory) + QString("untitled.rrwcp"),
                      "Rrwc profile (*.rrwcp);;All files (*)");

  loadProfile(filename);
}

MainWindow::~MainWindow() {
  delete p_logTimer;
  delete ui;
}
