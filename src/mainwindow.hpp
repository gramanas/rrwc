#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <QLineEdit>
#include "outputtab.hpp"
#include "outputmanager.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0,
               OutputManager *outputManager = nullptr);
    ~MainWindow();
private slots:
    void slotAddOutput();
    void slotGo();
    void slotRemoveOutput(int index);
    void slotBrowse(QLineEdit *line);
private:
    QVector<OutputTab *> m_outputTabs;
    OutputManager *p_outputManager;
    void connectButtons();
    void finalizeTabs();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
