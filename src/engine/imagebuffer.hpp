#ifndef IMAGEBUFFER_HPP
#define IMAGEBUFFER_HPP

#include <QObject>
#include <memory>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef QSharedPointer<cv::Mat> ImagePointer;

class ImageBuffer : public QObject {
    Q_OBJECT

  public:
    ImageBuffer(QStringList entryList);
    ~ImageBuffer();

    bool requestNext();
    const cv::Mat * getNext(int outputIndex);

  private:
    cv::Mat * loadNext();
    void releaseOld();
    bool isUsed(int index);

    QVector<std::unique_ptr<cv::Mat>> m_buffer;
    QVector<int> m_currentForOutput;
};


#endif // IMAGEBUFFER_HPP
