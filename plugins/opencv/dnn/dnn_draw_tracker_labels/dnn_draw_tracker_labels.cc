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

#include "dnn_draw_tracker_labels.h"
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements {

using namespace std;

DnnDrawTrackerLabels::DnnDrawTrackerLabels()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eIntVector, "Tracker IDs Vector", IOSocket::eCanHoldIntVector | IOSocket::eCanChangeName);
  addInput(ValueType::eDNNRectVector, "Rect vector", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);

  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
}

void DnnDrawTrackerLabels::calculate()
{
  auto image = m_inputs[0].getValue<cv::Mat>().clone();
  auto trackerIDs = m_inputs[1].getValue<IntVector>();
  auto rects = m_inputs[2].getValue<DNNRectVector>();

  unsigned trackerVecID = 0;

  if (image.size) {
    for (auto const &rect : rects) {
      cv::rectangle(image, rect.boundingBox, cv::Scalar(255, 178, 50), 3);

      std::string trackIDLabel{ "--MISSING ID--" };
      if (trackerVecID < trackerIDs.size()) {
        trackIDLabel = cv::format("ID: %i", trackerIDs[trackerVecID]);
        trackerVecID++;
      }

      int baseLine;
      auto top = rect.boundingBox.y;
      auto left = rect.boundingBox.x;

      cv::Size labelSize = getTextSize(trackIDLabel, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
      top = max(top, labelSize.height);
      cv::rectangle(image, cv::Point(left, static_cast<int>(top - round(1.5 * labelSize.height))),
                    cv::Point(static_cast<int>(left + round(1.5 * labelSize.width)), top + baseLine),
                    cv::Scalar(255, 255, 255), cv::FILLED);
      cv::putText(image, trackIDLabel, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(0, 0, 0), 1);
    }

    m_outputs[0].setValue(image);
  }
}

} // namespace spaghetti::elements
