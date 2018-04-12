#include "tui.hpp"

Tui::Tui(Rrwc* rrwc, const CliOptions &opt)
  : p_rrwc(rrwc),
    m_opt(opt) {
  initializeLogging();

  connect(p_rrwc, SIGNAL(done()), this, SLOT(onDone()));
  connect(p_rrwc, SIGNAL(started()), this, SLOT(onStarted()));
}

void Tui::initializeLogging() {
  p_logTimer = new QTimer();
  p_logTimer->setInterval(200);
  QObject::connect(p_logTimer, &QTimer::timeout, this, [=] {
      QString log = p_rrwc->logger()->flushLog();
      if (!log.isEmpty()) {
        console() << log << endl;
      }
      QString err = p_rrwc->logger()->flushErr();
      if (!err.isEmpty()) {
        console() << err << endl;
      }
      if (p_rrwc->logger()->tuiDead()) {
        exit(1);
      }
    });
  p_logTimer->start();
}

void Tui::onStarted() {
}

void Tui::onDone() {
  p_rrwc->logger()->done();
  p_rrwc->logger()->log("Job's done!");
  QTimer::singleShot(1000, this, []{exit(0);});
}

void Tui::exec() {
  if (!p_rrwc->outputManager()->loadProfile(m_opt.profile)) {
    p_rrwc->logger()->err(ERR_WRONG_PROFILE);
    p_rrwc->logger()->killTui();
  }
  else {
    p_rrwc->go(m_opt.input, m_opt.sortExif ? SORT_EXIF : SORT_FILENAME, m_opt.threads);
  }
}
