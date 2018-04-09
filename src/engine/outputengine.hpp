#ifndef OUTPUTENGINE_HPP
#define OUTPUTENGINE_HPP

#include <QStringList>
#include <QVector>
#include <QFileInfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "globals.hpp"
#include "engine/resizer.hpp"
#include "engine/marker.hpp"
#include "engine/renamer.hpp"
#include "output/output.hpp"
#include "logger.hpp"

// the engine's gears
struct Gears {
  Resizer resizer;
  Marker marker;
  Renamer renamer;

  // the data
  Output const *p_output = nullptr;
  bool copyFlag = false;
};

class OutputEngine : public QObject {
  Q_OBJECT
public:
  OutputEngine(const QVector<Output *> &outputs,
               const int &current,
               Logger *logger);
  ~OutputEngine();

  void init(Output const * output, int current);
  void setCurrentOutput(int index);
  bool loadImage(const QString &path);
  bool exec();
  bool write();

private:
  Logger *p_logger;

  int m_current;
  QString m_newName;
  QFileInfo m_sourceInfo;

  QVector<Gears *> m_gears;

  // current gear
  Gears * m_gear = nullptr;

  cv::Mat m_source;
  cv::Mat m_out;
};

#endif // OUTPUTENGINE_HPP
