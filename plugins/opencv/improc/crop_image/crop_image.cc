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

#include "crop_image.h"
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements {

using namespace std;

CropImage::CropImage()
  : Element{}
{
  setMinInputs(5);
  setMaxInputs(5);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "x", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "y", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "width", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "height", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
}

void CropImage::calculate()
{
  auto sourceImage = m_inputs[0].getValue<cv::Mat>();
  auto x = clamp(m_inputs[1].getValue<int>(), 0, sourceImage.cols - 1);
  auto y = clamp(m_inputs[2].getValue<int>(), 0, sourceImage.rows - 1);
  auto width = clamp(m_inputs[3].getValue<int>(), 0, sourceImage.cols - x - 1);
  auto height = clamp(m_inputs[4].getValue<int>(), 0, sourceImage.rows - y - 1);

  if (!sourceImage.empty()) {
    cv::Mat convertedImage{};

    convertedImage = sourceImage(cv::Rect(x, y, width, height)).clone();
    m_outputs[0].setValue(convertedImage);
  }
}

} // namespace spaghetti::elements
