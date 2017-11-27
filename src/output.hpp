#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <QString>

struct Counter {int counterStart;
    int start;
    int step;
    int digits;
};

struct Output {
    QString folder;
    bool resize;
    bool rename;
    bool watermark;
    QString renameText;
    QString watermarkText;
    int length;
    int height;
    int opacity;
    int threads;
    Counter counter;
};

#endif // OUTPUT_HPP
