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

#include "find_class.h"

namespace spaghetti::elements {

using namespace std;

FindClass::FindClass()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(3);
  setMaxOutputs(3);

  addInput(ValueType::eDNNRectVector, "DNN rect vector", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Class ID", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eBool, "Found", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "Ammount", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eDNNRectVector, "Items", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
}

void FindClass::calculate()
{
  auto dnnRects = m_inputs[0].getValue<DNNRectVector>();
  auto classID = m_inputs[1].getValue<int>();

  DNNRectVector outputRects{};
  for (auto const &rect : dnnRects) {
    if (rect.classId == classID) outputRects.push_back(rect);
  }

  m_outputs[0].setValue(outputRects.size() > 0 ? true : false);
  m_outputs[1].setValue(static_cast<int>(outputRects.size()));
  m_outputs[2].setValue(outputRects);
}

} // namespace spaghetti::elements
