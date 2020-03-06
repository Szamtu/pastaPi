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

#include "seven_segment_display.h"

namespace spaghetti::elements {

SevenSegmentDisplay::SevenSegmentDisplay()
{
  setMinInputs(8);
  setMaxInputs(8);
  setMinOutputs(0);
  setMaxOutputs(0);

  addInput(ValueType::eBool, "A", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "B", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "C", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "D", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "E", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "F", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "G", IOSocket::eCanHoldBool);
  addInput(ValueType::eBool, "DP", IOSocket::eCanHoldBool);
}

} // namespace spaghetti::elements
