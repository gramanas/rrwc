#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <QVector>
#include <QString>
#include "outputtab.hpp"

struct Output {
    QString folder;
    bool resize;
    bool rename;
    bool watermark;
    bool comment;
    QString resizeText;
    QString renameText;
    QString watermarkText;
    QString commentText;
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

#endif // OUTPUT_HPP
