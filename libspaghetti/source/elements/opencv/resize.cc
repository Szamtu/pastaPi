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

#include <spaghetti/elements/opencv/resize.h>
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements::opencv {
Resize::Resize()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "fx", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "fy", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
}

void Resize::calculate()
{
  auto matrix = std::get<SafeValue<cv::Mat>>(m_inputs[0].value);
  if (m_lastFrameTimeStamp != matrix.timeStamp()) {
    auto fx = static_cast<double>(std::get<float>(m_inputs[1].value));
    auto fy = static_cast<double>(std::get<float>(m_inputs[2].value));
    auto sourceImage = matrix.data();
    cv::Mat convertedImage{};

    if (!sourceImage.empty() && fx >= 0.1 && fy >= 0.1) {
      cv::resize(sourceImage, convertedImage, cv::Size(), fx, fy);

      m_outputs[0].value = convertedImage;
      m_lastFrameTimeStamp = matrix.timeStamp();
    }
  }
}

} // namespace spaghetti::elements::opencv
