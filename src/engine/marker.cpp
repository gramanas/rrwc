#include "engine/marker.hpp"

Marker::Marker() {
}

void Marker::loadData(QString watermarkPath, int opacity) {
    m_watermark = cv::imread(watermarkPath.toStdString(), cv::IMREAD_UNCHANGED);
    m_opacity = float(opacity) / 100;
}

bool Marker::exec(cv::Mat &destination) {
    if (m_finalWatermark.cols != p_source->cols) {
        cv::Rect watermarkArea(0,0,p_source->cols, p_source->rows);
        m_finalWatermark = cv::Mat(m_watermark, watermarkArea);
    }

    overlayImage(*p_source, m_finalWatermark, destination, cv::Point(0,0));
    return true;
}

void Marker::overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                          cv::Mat &output, cv::Point2i location) {
  background.copyTo(output);

  // start at the row indicated by location, or at row 0 if location.y is negative.
  for(int y = std::max(location.y , 0); y < background.rows; ++y)
  {
    int fY = y - location.y; // because of the translation

    // we are done of we have processed all rows of the foreground image.
    if(fY >= foreground.rows)
      break;

    // start at the column indicated by location,

    // or at column 0 if location.x is negative.
    for(int x = std::max(location.x, 0); x < background.cols; ++x)
    {
      int fX = x - location.x; // because of the translation.

      // we are done with this row if the column is outside of the foreground image.
      if(fX >= foreground.cols)
        break;

      // determine the opacity of the foregrond pixel, using its fourth (alpha) channel.
      double opacity =
        ((double)foreground.data[fY * foreground.step + fX * foreground.channels() + 3])

        / 255.;
      if (opacity != 0) {
          opacity-=(1.-m_opacity);
      }


      // and now combine the background and foreground pixel, using the opacity, 

      // but only if opacity > 0.
      for(int c = 0; opacity > 0 && c < output.channels(); ++c)
      {
        unsigned char foregroundPx =
          foreground.data[fY * foreground.step + fX * foreground.channels() + c];
        unsigned char backgroundPx =
          background.data[y * background.step + x * background.channels() + c];
        output.data[y*output.step + output.channels()*x + c] =
          backgroundPx * (1.-opacity) + foregroundPx * opacity;
      }
    }
  }
}

Marker::~Marker() {
}
