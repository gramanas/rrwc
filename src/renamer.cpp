#include "renamer.hpp"

Renamer::Renamer(const QString &originalName,
                 const QString &renameText,
                 const Counter &counter,
                 int currentPosition)
    : m_counter(counter),
      m_originalName(originalName),
      m_renameText(renameText),
      m_currentPosition(currentPosition) {
}

QString Renamer::formCounterString() {
    int currentCount = m_counter.start + (m_counter.step * m_currentPosition);
    QString ret = QString::number(currentCount);
    while (ret.size() < m_counter.digits){
        ret = QString::number(0) + ret;
    }
    return ret;
}

void Renamer::parseText() {
    bool flag = false;
    if (m_renameText.contains("[C]")) {
        m_renameText.replace(QString("[C]"), formCounterString());
        flag = true;
    }

    if (m_renameText.contains("[N]")) {
        m_renameText.replace(QString("[N]"), m_originalName);
        flag = true;
    }

    // if no special character is selected the counter will get appended
    // so there will be no errors while renaming
    if (flag == false) {
        m_renameText = m_renameText + "(" + formCounterString() + ")";
    }
}

void Renamer::exec(QString &newName) {
    parseText();
    newName = m_renameText;
}

Renamer::~Renamer() {
}


