#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <QLineEdit>
#include <QTimer>
#include "gui/outputtab.hpp"
#include "rrwc.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0,
             Rrwc *rrwc = nullptr);
  ~MainWindow();

private slots:
  void slotStatusChanged(QString status);
  void slotAddOutput();
  void slotToggleLogOutputs();
  void slotGo();
  void slotRemoveOutput(int index);
  void slotBrowse(QLineEdit *line);
  void actionHelp();
  void actionAbout();
  void actionSaveProfile();
  void actionLoadProfile();
  void onStarted();
  void onDone();
  void finalizeTabs();

private:
  void connectButtons();
  void enableLayout(bool t);

  QTimer * p_timer;
  QVector<OutputTab *> m_outputTabs;
  bool m_isRunning = false;
  Rrwc *p_rrwc;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
