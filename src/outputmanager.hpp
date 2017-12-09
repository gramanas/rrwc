#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>

#include "outputtab.hpp"
#include "enginemanager.hpp"
#include "output.hpp"

class OutputManager : public QObject {
    Q_OBJECT
  public:
    OutputManager();
    ~OutputManager();
    void generateOutputsFromTabs(QVector<OutputTab *> outputTabs);
    void saveProfile(const QString &filename);
    bool loadProfile(const QString &filename);
    void print() const;

    void startOutput(int output, const QString &inputPath, const QString &sort);
    void clean();

    QVector<Output *> outputs() const {
        return m_outputs;
    }

  public slots:
    void onProgressChanged(int output, int progress);
    void onDone();

  signals:
    void progressChanged(int progress);
    void done();


  private:
    void fillEntryList(QDir dir, const QString &sort);

    QStringList m_entryList;
    QVector<Output *> m_outputs;
    QVector<int> m_outputProgress;
    QVector<EngineManager *> m_engines;
    int m_outputsRemaining;
};

#endif // OUTPUTMANAGER_HPP
