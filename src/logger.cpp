#include <QDebug>
#include "logger.hpp"

Logger::Logger() {
}

Logger::~Logger() {
}

void Logger::append(QMutex &m, QString &s, const QString &n) {
  QMutexLocker locker(&m);
  if (!s.isEmpty()) {
    s.append("\n" + n);
    return;
  }
  s.append(n);
}

void Logger::replace(QMutex &m, QString &s, const QString &n) {
  QMutexLocker locker(&m);
  s = n;
}

QString Logger::flush(QMutex &m, QString &s) {
  QMutexLocker locker(&m);
  QString ret = s;
  s.clear();
  return ret;
}

void Logger::killTui() {
  QMutexLocker locker(&m_killMutex);
  m_killTui = true;
}

bool Logger::tuiDead() {
  QMutexLocker locker(&m_killMutex);
  return m_killTui;
}

void Logger::err(QString str) {
  append(m_errMutex, m_errors, str);
}

void Logger::log(QString str) {
  append(m_logMutex, m_logs, str);
}

void Logger::status(QString str) {
  replace(m_pBarStatusMutex, m_pBarStatus, str);
}

void Logger::incrementItemsDone() {
  m_itemsDone.fetchAndAddOrdered(1);
}

void Logger::setItemsDone(uint n) {
  m_itemsDone.fetchAndStoreOrdered(n);
}

void Logger::setTotal(uint n) {
  m_total.fetchAndStoreOrdered(n);
}

QString Logger::flushLog() {
  return flush(m_logMutex, m_logs);
}

QString Logger::flushErr() {
  return flush(m_errMutex, m_errors);
}

QString Logger::flushStatus() {
  return flush(m_pBarStatusMutex, m_pBarStatus);
}

uint Logger::getProgress() {
  return uint(( (float) m_itemsDone.fetchAndAddOrdered(0) /
                (float) m_total.fetchAndAddOrdered(0) ) * 100);
}
