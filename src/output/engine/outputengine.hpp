#ifndef OUTPUTENGINE_HPP
#define OUTPUTENGINE_HPP

#include <QStringList>
#include <QFileInfo>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "globals.hpp"
#include "output/engine/resizer.hpp"
#include "output/engine/marker.hpp"
#include "output/engine/renamer.hpp"
#include "output/output.hpp"

class OutputEngine : public QObject {
    Q_OBJECT
  public:
    // loads the rules
    OutputEngine();
    ~OutputEngine();

    void init(Output const * output, int current);
    bool loadImage(const QString &path);
    bool exec();
    bool write();

  signals:
    void writeLog(QString log, QString str);

  private:
    Output const *p_output;
    bool m_copyFlag;

    Resizer m_resizer;
    Marker m_marker;
    Renamer m_renamer;

    QFileInfo m_sourceInfo;
    int m_current;
    QString m_newName;

    cv::Mat m_source;
    cv::Mat m_watermark;
    cv::Mat m_temp;
    cv::Mat m_out;
};

#endif // OUTPUTENGINE_HPP
