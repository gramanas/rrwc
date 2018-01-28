#ifndef RENAMER_HPP
#define RENAMER_HPP

#include "output/output.hpp"
#include "output/engine/worker.hpp"

class Renamer : public Worker {
  public:
    Renamer();
    ~Renamer();

    void loadData(const QString &renameText, const Counter &counter);
    void loadNext(const QString &originalName,
                  int currentPos);
    void exec(QString &newName);

  private:
    QString parseText();
    QString formCounterString();

    Counter m_counter;
    QString m_renameText;

    QString m_originalName;
    int m_currentPosition;
};

#endif // RENAMER_HPP
