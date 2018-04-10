#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>

#include "gui/outputtab.hpp"
#include "output/entrylist.hpp"
#include "output/output.hpp"
#include "engine/threadmanager.hpp"
#include "logger.hpp"


class OutputManager : public QObject {
  Q_OBJECT
public:
  OutputManager(Logger *logger);
  ~OutputManager();
  void print() const;

  void saveProfile(const QString &filename);
  bool loadProfile(const QString &filename);

  void generateOutputsFromTabs(QVector<OutputTab *> outputTabs);
  void fillEntryList(const QString &inputPath, const QString &sort);
  void startOutputs(const int &threads);
  void clean();

  QVector<Output *> outputs() const {
    return m_outputs;
  }

public slots:
  void slotEntryListFull();
  void onDone();

signals:
  void entryListFull();
  void done();

private:
  Logger *p_logger;
  EntryList m_entryList;
  QVector<Output *> m_outputs;
  ThreadManager *p_threadManager;
};

#endif // OUTPUTMANAGER_HPP
