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

#include "unpack_rect.h"

namespace spaghetti::elements {

using namespace std;

UnpackRect::UnpackRect()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(6);
  setMaxOutputs(6);

  addInput(ValueType::eDNNRect, "DNN rect", IOSocket::eCanHoldDNNRect | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "X", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "Y", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "Width", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "Height", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "ClassID", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eFloat, "Confidence", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
}

void UnpackRect::calculate()
{
  auto rect = m_inputs[0].getValue<DNNRect>();

  m_outputs[0].setValue(rect.boundingBox.x);
  m_outputs[1].setValue(rect.boundingBox.y);
  m_outputs[2].setValue(rect.boundingBox.width);
  m_outputs[3].setValue(rect.boundingBox.height);
  m_outputs[4].setValue(rect.classId);
  m_outputs[5].setValue(rect.confidence);
}

} // namespace spaghetti::elements
