#ifndef OUTPUTTAB_HPP
#define OUTPUTTAB_HPP

#include <QWidget>
#include <QLineEdit>

namespace Ui {
class OutputTab;
}

class OutputTab : public QWidget
{
    Q_OBJECT

  public:
    explicit OutputTab(QWidget *parent = 0);
    ~OutputTab();
    Ui::OutputTab *getUi() const{
        return ui;
    }
  private slots:
    void slotBrowse(QLineEdit *line);
    void slotBrowseWatermark(QLineEdit *line);
  private:
    void connectButtons();
    Ui::OutputTab *ui;
};

#endif // OUTPUTTAB_HPP
