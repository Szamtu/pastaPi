// MIT License
//
// Copyright (c) 2020 Piotr Zdanowicz
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

#include "addition.h"
#include <QVariant>

namespace spaghetti::elements {

Addition::Addition()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eInt, "#1", IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "#2", IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "Out", IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  setDefaultNewOutputFlags(IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
}

void Addition::calculate()
{
  auto inputTypeA = m_inputs[0].type;
  auto inputTypeB = m_inputs[1].type;

  auto outputType = m_outputs[0].type;

  QVariant inputA{};
  QVariant inputB{};

  switch (inputTypeA) {
    case ValueType::eInt: inputA = m_inputs[0].getValue<int>(); break;
    case ValueType::eFloat: inputA = m_inputs[0].getValue<double>(); break;
    default: assert(false);
  }

  switch (inputTypeB) {
    case ValueType::eInt: inputB = m_inputs[1].getValue<int>(); break;
    case ValueType::eFloat: inputB = m_inputs[1].getValue<double>(); break;
    default: assert(false);
  }

  switch (outputType) {
    case ValueType::eInt: m_outputs[0].setValue(inputA.toInt() + inputB.toInt()); break;
    case ValueType::eFloat: m_outputs[0].setValue(inputA.toFloat() - inputB.toFloat()); break;
    default: assert(false);
  }
}

} // namespace spaghetti::elements
