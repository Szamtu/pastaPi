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

#include "draw_contours.h"
#include <QDebug>
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements {
DrawContours::DrawContours()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eShapeVector, "Contours", IOSocket::eCanHoldShapeVector | IOSocket::eCanChangeName);
  setDefaultNewInputFlags(IOSocket::eCanHoldShapeVector | IOSocket::eCanChangeName);

  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
}

void DrawContours::calculate()
{
  auto sourceImage = m_inputs[0].getValue<cv::Mat>();
  auto shapeVec = m_inputs[1].getValue<ShapeVector>();

  if (sourceImage.empty()) {
    auto convertedImage = sourceImage.clone();
    cv::RNG rng(12345);

    for (size_t i = 0; i < shapeVec.size(); i++) {
      cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      drawContours(convertedImage, shapeVec, static_cast<int>(i), color, 2, 8);
    }

    m_outputs[0].setValue(convertedImage);
  }
}

} // namespace spaghetti::elements
