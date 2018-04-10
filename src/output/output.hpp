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
  bool comment;
  bool stripMetadata;
  QString renameText;
  QString watermarkText;
  QString commentText;
  int length;
  int height;
  int opacity;
  Counter counter;
  int index;
};

#endif // OUTPUT_HPP
