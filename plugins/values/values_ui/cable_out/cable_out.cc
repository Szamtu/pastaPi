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

#include "cable_out.h"

namespace spaghetti::elements {

CableOut::CableOut()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(1);

  addInput(ValueType::eCable, "#1", IOSocket::eCanHoldCable | IOSocket::eCanChangeName);
  addOutput(ValueType::eBool, "#1", IOSocket::eCanHoldAllValues | IOSocket::eCanChangeName);

  setDefaultNewOutputFlags(IOSocket::eCanHoldAllValues | IOSocket::eCanChangeName);
}

void CableOut::calculate()
{
  auto cable = m_inputs[0].getValue<Cable>();

  for (unsigned i = 0; i < cable.size() && i < outputs().size(); i++) {
    auto value = std::any_cast<Value>(cable[i]);
    if (value.index() == outputs()[i].getRawValueIndex()) {
      outputs()[i].setValue(value);
    }
  }
}

} // namespace spaghetti::elements
