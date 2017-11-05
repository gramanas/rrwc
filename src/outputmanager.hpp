#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <QVector>
#include <QString>
#include "outputtab.hpp"

struct Output {
    QString folder;
    bool active;
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
    void generateOutputs(QVector<OutputTab *> outputTabs);
    QVector<Output *> outputs() const;

  private:
    QVector<Output *> m_outputs;
};

#endif // OUTPUT_HPP
