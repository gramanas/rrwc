#ifndef RENAMER_HPP
#define RENAMER_HPP

#include "outputmanager.hpp"

class Renamer {
  public:
    Renamer(const QString &originalName,
            const QString &renameText,
            const Counter &counter,
            int currentPosition);
    ~Renamer();

    void exec(QString &newName);

  private:
    void parseText();
    QString formCounterString();

    const QString m_originalName;
    const Counter m_counter;
    const int m_currentPosition;
    QString m_renameText;
};

#endif // RENAMER_HPP
