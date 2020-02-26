// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
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

#include "nand.h"

namespace spaghetti::elements {

Nand::Nand()
  : Element{}
{
  setMinInputs(2);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eBool, "#1", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "#2", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void Nand::calculate()
{
  bool allSets{ true };
  for (auto &input : m_inputs) {
    bool const VALUE{ input.getValue<bool>() };
    if (!VALUE) {
      allSets = false;
      break;
    }
  }

  m_outputs[0].setValue(!allSets);
}

} // namespace spaghetti::elements
