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

#include "imwrite.h"
#include <opencv2/imgcodecs.hpp>

namespace spaghetti::elements {
ImWrite::ImWrite()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(0);
  setMaxOutputs(0);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eString, "Path", IOSocket::eCanHoldString | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "Trigger", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void ImWrite::calculate()
{
  auto path = m_inputs[1].getValue<std::string>();
  auto trigger = m_inputs[2].getValue<bool>();
  auto sourceImage = m_inputs[0].getValue<cv::Mat>();

  if (trigger && !m_lastState && !path.empty() && !sourceImage.empty()) {
    cv::imwrite(path, sourceImage);
  }
  m_lastState = trigger;
}

} // namespace spaghetti::elements
