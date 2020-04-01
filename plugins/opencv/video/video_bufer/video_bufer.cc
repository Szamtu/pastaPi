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

#include "video_bufer.h"
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements {

using namespace std;

VideoBufer::VideoBufer()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eMatrix, "Frame", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "BufMaxSize", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "Clear", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);

  addOutput(ValueType::eMatrixVector, "Video", IOSocket::eCanHoldMatrixVector | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "CurentSize", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
}

void VideoBufer::calculate()
{
  auto frame = m_inputs[0].getValue<cv::Mat>();
  auto maxSize = m_inputs[1].getValue<int>();
  auto clear = m_inputs[2].getValue<bool>();

  if (maxSize < 0) maxSize = 0;

  if (clear) {
    m_bufer.clear();
    m_outputs[0].setValue(m_bufer);
    m_outputs[1].setValue(0);

    return;
  }

  if (m_bufer.size() > static_cast<size_t>(maxSize)) {
    m_bufer.erase(m_bufer.begin(), m_bufer.begin() + static_cast<int>(m_bufer.size()) - maxSize);
  }

  if (!frame.empty() && maxSize > 0) {
    if (m_bufer.size() == static_cast<size_t>(maxSize)) {
      m_bufer.erase(m_bufer.begin());
    }
    m_bufer.push_back(frame);
  }

  m_outputs[0].setValue(m_bufer);
  m_outputs[1].setValue(static_cast<int>(m_bufer.size()));
}

} // namespace spaghetti::elements
