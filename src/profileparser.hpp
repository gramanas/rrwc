#ifndef PROFILEPARSER_HPP
#define PROFILEPARSER_HPP

#include <QFile>
#include <QVector>

#include "output.hpp"

class ProfileParser {
  public:
    ProfileParser(const QVector<Output *> outputs);
    ProfileParser();
    ~ProfileParser();

    void writeToFile(const QString &filename);
    bool readFromFile(const QString &filename, QVector<Output *> &vector);
    void copyOutputsToVector(QVector<Output *> &vector);
    void parse(int &output, QStringList &data);
    void parse(QString &output, QStringList &data);
    void parse(bool &output, QStringList &data);

  private:
    QStringList stripWhitespaces(QStringList list);

    QVector<Output *> m_outputs;
};

#endif // PROFILEPARSER_HPP
