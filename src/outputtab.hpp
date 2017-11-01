#ifndef OUTPUTTAB_HPP
#define OUTPUTTAB_HPP

#include <QWidget>

namespace Ui {
class OutputTab;
}

class OutputTab : public QWidget
{
    Q_OBJECT

  public:
    explicit OutputTab(QWidget *parent = 0);
    ~OutputTab();
  private slots:
    void slotBrowse(QLineEdit *line);
    void slotBrowseWatermark(QLineEdit *line);
  private:
    void connectButtons();
    Ui::OutputTab *ui;
};

#endif // OUTPUTTAB_HPP
