#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>
#include "outputtab.hpp"

struct Output {
    QString folder;
    bool resize;
    bool rename;
    bool watermark;
    bool comment;
    QString renameText;
    QString watermarkText;
    QString commentText;
    int length;
    int height;
    int opacity;
    int counterStart;
    int counterStep;
    int counterDigit;
};

class OutputManager {
  public:
    OutputManager();
    ~OutputManager();
    void generateOutputsFromTabs(QVector<OutputTab *> outputTabs);
    void print() const;

    QVector<Output *> outputs() const {
        return m_outputs;
    }

  private:
    QVector<Output *> m_outputs;
};

#endif // OUTPUTMANAGER_HPP
