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

#include "const_string.h"

namespace spaghetti::elements {

ConstString::ConstString()
{
  setMinInputs(0);
  setMaxInputs(0);
  setMinOutputs(1);
  setMaxOutputs(1);

  addOutput(ValueType::eString, "Value", IOSocket::eCanHoldString | IOSocket::eCanChangeName);
}

void ConstString::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["value"] = m_currentValue;
}

void ConstString::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];
  m_currentValue = PROPERTIES["value"].get<std::string>();

  m_outputs[0].setValue(m_currentValue);
}

void ConstString::set(std::string const a_value)
{
  m_currentValue = a_value;
  m_outputs[0].setValue(m_currentValue);
}

} // namespace spaghetti::elements
