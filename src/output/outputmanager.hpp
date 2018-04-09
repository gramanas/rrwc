#ifndef OUTPUTMANAGER_HPP
#define OUTPUTMANAGER_HPP

#include <QVector>
#include <QString>

#include "gui/outputtab.hpp"
#include "output/entrylist.hpp"
#include "output/output.hpp"
#include "engine/threadmanager.hpp"


class OutputManager : public QObject {
  Q_OBJECT
public:
  OutputManager();
  ~OutputManager();
  void generateOutputsFromTabs(QVector<OutputTab *> outputTabs);
  void saveProfile(const QString &filename);
  bool loadProfile(const QString &filename);
  void print() const;

  void startOutputs(const int &threads);
  void clean();
  void fillEntryList(const QString &inputPath, const QString &sort);
  int progress() {
    return int(((float)p_threadManager->getItemsDone()/(float)m_entryList.get()->count()) * 100);
  }

  QVector<Output *> outputs() const {
    return m_outputs;
  }

public slots:
  void onStatusChanged(QString status);
  void slotEntryListFull();
  void onDone();

signals:
  void statusChanged(QString status);
  void entryListFull();
  void done();

private:
  EntryList m_entryList;
  QVector<Output *> m_outputs;
  ThreadManager *p_threadManager;
};

#endif // OUTPUTMANAGER_HPP
