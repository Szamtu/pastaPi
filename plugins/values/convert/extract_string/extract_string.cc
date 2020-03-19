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

#include "extract_string.h"
#include <QVariant>

namespace spaghetti::elements {
ExtractString::ExtractString()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eStringVector, "String vector", IOSocket::eCanHoldStringVector | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Index", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eString, "String", IOSocket::eCanHoldString | IOSocket::eCanChangeName);
}

void ExtractString::calculate()
{
  auto const STRING_VECTOR = m_inputs[0].getValue<StringVector>();
  auto const INDEX = static_cast<size_t>(m_inputs[1].getValue<int>());

  if (INDEX < STRING_VECTOR.size()) {
    m_outputs[0].setValue(STRING_VECTOR[INDEX]);
  } else {
    m_outputs[0].setValue(std::string());
  }
}

} // namespace spaghetti::elements
