// MIT License
//
// Copyright (c) 2020 Paweł Adamski
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "nodes/opencv/videodisplay.h"
#include <spaghetti/elements/opencv/videodisplay.h>

#include <QGraphicsSimpleTextItem>
#include <QTableWidget>

#include <opencv4/opencv2/imgproc.hpp>

namespace spaghetti::nodes::opencv {

VideoDisplay::VideoDisplay()
{
  m_image = new QGraphicsPixmapItem{};
  setCentralWidget(m_image);
}

void VideoDisplay::refreshCentralWidget()
{
  if (!m_element) return;

  auto frame = m_element->inputs()[0].getValue<cv::Mat>();

  if (!frame.empty()) {
    cv::Mat convertedFrame{};
    cv::cvtColor(frame, convertedFrame, cv::COLOR_BGR2RGB);

    QImage image{ convertedFrame.data, convertedFrame.cols, convertedFrame.rows, static_cast<int>(convertedFrame.step),
                  QImage::Format_RGB888 };
    QPixmap pixmap = QPixmap::fromImage(image);

    if (!image.isNull()) {
      m_image->setPixmap(pixmap);
    }
  }

  calculateBoundingRect();
}

void VideoDisplay::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

} // namespace spaghetti::nodes::opencv
