#include "output/engine/renamer.hpp"

Renamer::Renamer() {
}

void Renamer::loadData(const QString &renameText,
                       const Counter &counter) {
    m_counter = counter;
    m_renameText =renameText;
}

void Renamer::loadNext(const QString &originalName,
                       int currentPos) {
    m_originalName = originalName;
    m_currentPosition = currentPos;
}

QString Renamer::formCounterString() {
    int currentCount = m_counter.start + (m_counter.step * m_currentPosition);
    QString ret = QString::number(currentCount);
    while (ret.size() < m_counter.digits){
        ret = QString::number(0) + ret;
    }
    return ret;
}

QString Renamer::parseText() {
    bool flag = false;
    QString temp = m_renameText;
    if (temp.contains("[C]")) {
        temp.replace(QString("[C]"), formCounterString());
        flag = true;
    }

    if (temp.contains("[N]")) {
        temp.replace(QString("[N]"), m_originalName);
        flag = true;
    }

    // if no special character is selected the counter will get prepended
    // so there will be no errors while renaming
    if (flag == false) {
        temp = "(" + formCounterString() + ")" + temp;
    }
    return temp;
}

void Renamer::exec(QString &newName) {
    newName = parseText();
}

Renamer::~Renamer() {
}


