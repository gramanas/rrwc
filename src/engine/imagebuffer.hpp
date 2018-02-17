#ifndef IMAGEBUFFER_HPP
#define IMAGEBUFFER_HPP

#include <QObject>
#include <memory>
#include <QVector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef std::unique_ptr<cv::Mat> ImagePointer;

class ImageBuffer : public QObject {
    Q_OBJECT

  public:
    ImageBuffer(const QStringList &entryList, int ountputNumber);
    ~ImageBuffer();

    const cv::Mat * getNext(int outputIndex);

  private:
    void loadNextBlock();
    ImagePointer load(int i);
    void releaseOld();

    QVector<ImagePointer> m_buffer;
    QVector<int> m_currentForOutput;
    int m_lastUsedImage = 0;
    int m_outputHead = 0;
    int m_bufferHead = 0;
    int m_blockSize;

    const QStringList m_entryList;
};


#endif // IMAGEBUFFER_HPP
