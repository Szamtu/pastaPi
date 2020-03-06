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

#include "ceil.h"
#include <QVariant>
#include "qmath.h"

namespace spaghetti::elements {

Ceil::Ceil()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eFloat, "A", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addOutput(ValueType::eInt, "Out", IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  setDefaultNewOutputFlags(IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
}

void Ceil::calculate()
{
  auto inputType = m_inputs[0].type;
  auto outputType = m_outputs[0].type;

  QVariant input{};

  switch (inputType) {
    case ValueType::eInt: input = m_inputs[0].getValue<int>(); break;
    case ValueType::eFloat: input = m_inputs[0].getValue<double>(); break;
    default: assert(false);
  }

  switch (outputType) {
    case ValueType::eInt: m_outputs[0].setValue((int)qCeil(input.toReal())); break;
    case ValueType::eFloat: m_outputs[0].setValue((double)qCeil(input.toReal())); break;
    default: assert(false);
  }
}

} // namespace spaghetti::elements
