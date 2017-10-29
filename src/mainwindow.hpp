#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLineEdit>

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
    void slotBrowse(QLineEdit *line);
    void slotBrowseWatermark(QLineEdit *line);
private:
    void connectButtons();
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_HPP
