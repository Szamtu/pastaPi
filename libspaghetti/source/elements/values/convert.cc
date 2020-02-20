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

#include <spaghetti/elements/values/convert.h>
#include <QVariant>

namespace spaghetti::elements::values {
Convert::Convert()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eInt, "In",
           IOSocket::eCanHoldBool | IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanHoldString |
               IOSocket::eCanChangeName);
  addOutput(ValueType::eString, "Out",
            IOSocket::eCanHoldBool | IOSocket::eCanHoldInt | IOSocket::eCanHoldFloat | IOSocket::eCanHoldString |
                IOSocket::eCanChangeName);
}

void Convert::calculate()
{
  auto inputType = m_inputs[0].type;
  auto outputType = m_outputs[0].type;

  QVariant input{};
  switch (inputType) {
    case ValueType::eBool: input = std::get<bool>(m_inputs[0].value); break;
    case ValueType::eInt: input = std::get<int>(m_inputs[0].value); break;
    case ValueType::eFloat: input = std::get<float>(m_inputs[0].value); break;
    case ValueType::eString: input = QString::fromStdString(std::get<std::string>(m_inputs[0].value)); break;
    default: assert(false);
  }

  switch (outputType) {
    case ValueType::eBool: m_outputs[0].value = input.toBool(); break;
    case ValueType::eInt: m_outputs[0].value = input.toInt(); break;
    case ValueType::eFloat: m_outputs[0].value = input.toFloat(); break;
    case ValueType::eString: m_outputs[0].value = input.toString().toStdString(); break;
    default: assert(false);
  }
}

} // namespace spaghetti::elements::values
