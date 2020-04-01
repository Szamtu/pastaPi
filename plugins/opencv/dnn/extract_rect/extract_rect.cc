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

#include "extract_rect.h"

namespace spaghetti::elements {

using namespace std;

ExtractRect::ExtractRect()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(3);
  setMaxOutputs(3);

  addInput(ValueType::eDNNRectVector, "DNN rect vector", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Vector ID", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eDNNRect, "Rect", IOSocket::eCanHoldDNNRect | IOSocket::eCanChangeName);
  addOutput(ValueType::eBool, "Out of scope", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void ExtractRect::calculate()
{
  auto dnnRects = m_inputs[0].getValue<DNNRectVector>();
  auto vectorID = m_inputs[1].getValue<int>();

  size_t id{};
  if (vectorID < 0) {
    id = dnnRects.size() + 1;
  } else {
    id = static_cast<size_t>(vectorID);
  }

  if (id < dnnRects.size()) {
    m_outputs[0].setValue(dnnRects[id]);
    m_outputs[1].setValue(false);
  } else {
    m_outputs[0].setValue(DNNRect());
    m_outputs[1].setValue(true);
  }
}

} // namespace spaghetti::elements
