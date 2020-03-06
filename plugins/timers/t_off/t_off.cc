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

#include "t_off.h"
#include <spaghetti/logger.h>

namespace spaghetti::elements {

TimerOff::TimerOff()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eBool, "Input", IOSocket::eCanHoldBool);
  addInput(ValueType::eInt, "Preset [ms]", IOSocket::eCanHoldInt);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool);
  addOutput(ValueType::eInt, "Elapsed [ms]", IOSocket::eCanHoldInt);
}

void TimerOff::update(duration_t const &a_delta)
{
  if (m_state == State::eRun) {
    m_elapsedTime += a_delta;
    if (m_elapsedTime >= m_presetTime) m_state = State::eDone;
  }
}

void TimerOff::calculate()
{
  bool const INPUT = m_inputs[0].getValue<bool>();
  int32_t const PRESET_MS = m_inputs[1].getValue<int32_t>();
  duration_t const PRESET = duration_t{ PRESET_MS };

  if (PRESET != m_presetTime) m_presetTime = PRESET;

  if (INPUT != m_lastInput) {
    m_state = !INPUT ? State::eRun : State::eReset;
    m_lastInput = INPUT;
  }

  switch (m_state) {
    case State::eWaitForTrigger: break;
    case State::eRun:
      m_outputs[0].setValue(true);
      m_outputs[1].setValue(
          static_cast<int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(m_elapsedTime).count()));
      break;
    case State::eDone:
      m_outputs[0].setValue(false);
      m_outputs[1].setValue(
          static_cast<int32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(m_elapsedTime).count()));
      break;
    case State::eReset:
      m_elapsedTime = duration_t{ 0 };
      m_outputs[0].setValue(false);
      m_outputs[1].setValue(0);
      m_state = State::eWaitForTrigger;
      break;
  }
}

} // namespace spaghetti::elements
