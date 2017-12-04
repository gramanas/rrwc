#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <QLineEdit>
#include "outputtab.hpp"
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
    void slotAddOutput();
    void slotGo();
    void slotRemoveOutput(int index);
    void slotBrowse(QLineEdit *line);
    void actionHelp();
    void actionSaveProfile();
    void actionLoadProfile();
    void onStarted();
    void onDone();

  private:
    void connectButtons();
    void finalizeTabs();
    void enableLayout(bool t);

    QVector<OutputTab *> m_outputTabs;
    Rrwc *p_rrwc;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
