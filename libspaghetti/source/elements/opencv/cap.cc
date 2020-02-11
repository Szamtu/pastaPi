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

#include <spaghetti/elements/opencv/cap.h>

namespace spaghetti::elements::opencv {
Cap::Cap()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eString, "Source", IOSocket::eCanHoldString | IOSocket::eCanChangeName);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void Cap::calculate()
{
  if (!m_cap.isOpened()) {
    auto const value{ std::get<std::string>(m_inputs[0].value) };

    if (!value.empty()) m_cap.open(value);
  }

  if (m_cap.isOpened()) {
    m_outputs[0].value = true;

    cv::Mat image{};
    m_cap.read(image);
    m_outputs[1].value = image;
  }
}

} // namespace spaghetti::elements::opencv
