#ifndef TUI_HPP
#define TUI_HPP

#include <QObject>
#include <QTimer>
#include <QTextStream>

#include "rrwc.hpp"
#include "cliparser.hpp"

class Tui : public QObject {
  Q_OBJECT
public:
  Tui(Rrwc *rrwc, const CliOptions &opt);
  void exec();

  inline QTextStream &console() {
    static QTextStream S{stdout};
    return S;
  }

private slots:
  void onStarted();
  void onDone();

private:
  void initializeLogging();

  Rrwc *p_rrwc;
  QTimer *p_logTimer;
  const CliOptions m_opt;
};

#endif // TUI_HPP
