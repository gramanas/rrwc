#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>
#include "outputtab.hpp"

struct Counter {int counterStart;
    int start;
    int step;
    int digits;
};

struct Output {
    QString folder;
    bool resize;
    bool rename;
    bool watermark;
    QString renameText;
    QString watermarkText;
    int length;
    int height;
    int opacity;
    int threads;
    Counter counter;
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
