#ifndef ERRORMANAGER_HPP
#define ERRORMANAGER_HPP

#include "output/outputmanager.hpp"
#include "logger.hpp"

class ErrorManager : public QObject {
  Q_OBJECT
public:
  ErrorManager(OutputManager *outputManager,
               Logger *logger);
  ~ErrorManager();

  bool checkInputErrors(const QString &inputPath);

private:
  Logger *p_logger;
  OutputManager *p_outputManager;
  bool m_flag = true;
};

#endif // ERRORMANAGER_HPP
