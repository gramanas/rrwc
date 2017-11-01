#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QVector>
#include <QLineEdit>
#include "outputtab.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void slotAddOutput();
    void slotRemoveOutput(int index);
    void slotBrowse(QLineEdit *line);
private:
    QVector<OutputTab *> m_outputs;
    void connectButtons();
    void finalizeTabs();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
