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

#include <spaghetti/elements/opencv/medianblur.h>
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements::opencv {
MedianBlur::MedianBlur()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "kSize", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
}

void MedianBlur::calculate()
{
  auto matrix = std::get<SafeValue<cv::Mat>>(m_inputs[0].value);
  if (m_lastFrameTimeStamp != matrix.timeStamp()) {
    auto sourceImage = matrix.data();

    if (!sourceImage.empty()) {
      auto kSize = std::get<int>(m_inputs[1].value);
      cv::Mat convertedImage{};

      if (kSize % 2 == 0) kSize--;
      if (kSize < 3) kSize = 3;

      cv::medianBlur(sourceImage, convertedImage, kSize);
      m_outputs[0].value = convertedImage;
    }

    m_lastFrameTimeStamp = matrix.timeStamp();
  }
}

} // namespace spaghetti::elements::opencv
