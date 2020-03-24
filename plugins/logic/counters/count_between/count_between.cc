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

#include "count_between.h"
#include <algorithm>

namespace spaghetti::elements {

CountBetween::CountBetween()
  : Element{}
{
  setMinInputs(5);
  setMaxInputs(5);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "Count", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "Reset", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Min", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Max", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "Direction", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "Value", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void CountBetween::calculate()
{
  auto const COUNT = m_inputs[0].getValue<bool>();
  auto const RESET = m_inputs[1].getValue<bool>();
  auto const MIN = m_inputs[2].getValue<int>();
  auto const MAX = m_inputs[3].getValue<int>();
  auto const DIRECTION = m_inputs[4].getValue<bool>();

  if (COUNT && !m_lastState) {
    if (DIRECTION) {
      m_value++;
    } else {
      m_value--;
    }
  }

  m_lastState = COUNT;
  if (RESET) m_value = MIN;

  m_value = std::clamp(m_value, MIN, MAX);

  m_outputs[0].setValue(m_value);
}

} // namespace spaghetti::elements
