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

#include "spaghetti/element.h"

#include <cassert>
#include <iostream>

#include <opencv2/core/mat.hpp>
#include "spaghetti/package.h"

namespace spaghetti {

void Element::serialize(Element::Json &a_json)
{
  auto &jsonElement = a_json["element"];
  jsonElement["id"] = m_id;
  jsonElement["name"] = m_name;
  jsonElement["type"] = type();
  jsonElement["min_inputs"] = m_minInputs;
  jsonElement["max_inputs"] = m_maxInputs;
  jsonElement["min_outputs"] = m_minOutputs;
  jsonElement["max_outputs"] = m_maxOutputs;
  jsonElement["default_new_input_flags"] = m_defaultNewInputFlags;
  jsonElement["default_new_output_flags"] = m_defaultNewOutputFlags;

  auto jsonInputs = Json::array();
  size_t const INPUTS_COUNT{ m_inputs.size() };
  for (size_t i = 0; i < INPUTS_COUNT; ++i) {
    Json socket{};
    socket["socket"] = i;
    socket["type"] = ValueDescription::typeCString(m_inputs[i].type);
    socket["name"] = m_inputs[i].name;
    socket["flags"] = m_inputs[i].flags;
    jsonInputs.push_back(socket);
  }

  auto jsonOutputs = Json::array();
  size_t const OUTPUTS_COUNT{ m_outputs.size() };
  for (size_t i = 0; i < OUTPUTS_COUNT; ++i) {
    Json socket{};
    socket["socket"] = i;
    socket["type"] = ValueDescription::typeCString(m_outputs[i].type);
    socket["name"] = m_outputs[i].name;
    socket["flags"] = m_outputs[i].flags;
    jsonOutputs.push_back(socket);
  }

  auto &jsonIo = jsonElement["io"];
  jsonIo["inputs"] = jsonInputs;
  jsonIo["outputs"] = jsonOutputs;

  auto &jsonNode = a_json["node"];
  jsonNode["position"]["x"] = m_position.x;
  jsonNode["position"]["y"] = m_position.y;
  jsonNode["iconify"] = m_isIconified;
  jsonNode["iconifying_hides_central_widget"] = m_iconifyingHidesCentralWidget;
}

void Element::deserialize(Json const &a_json)
{
  auto const &ELEMENT = a_json["element"];
  auto const NAME = ELEMENT["name"].get<std::string>();
  auto const MIN_INPUTS = ELEMENT["min_inputs"].get<uint64_t>();
  auto const MAX_INPUTS = ELEMENT["max_inputs"].get<uint64_t>();
  auto const MIN_OUTPUTS = ELEMENT["min_outputs"].get<uint64_t>();
  auto const MAX_OUTPUTS = ELEMENT["max_outputs"].get<uint64_t>();
  auto const DEFAULT_NEW_INPUT_FLAGS = ELEMENT["default_new_input_flags"].get<uint64_t>();
  auto const DEFAULT_NEW_OUTPUT_FLAGS = ELEMENT["default_new_output_flags"].get<uint64_t>();

  auto const &IO = ELEMENT["io"];
  auto const &INPUTS = IO["inputs"];
  auto const &OUTPUTS = IO["outputs"];

  auto const &NODE = a_json["node"];
  auto const ICONIFY = NODE["iconify"].get<bool>();
  auto const ICONIFYING_HIDES_CENTRAL_WIDGET = NODE["iconifying_hides_central_widget"].get<bool>();
  auto const &POSITION = NODE["position"];
  auto const POSITION_X = POSITION["x"].get<double>();
  auto const POSITION_Y = POSITION["y"].get<double>();

  setName(NAME);
  setPosition(POSITION_X, POSITION_Y);
  clearInputs();
  clearOutputs();
  setMinInputs(MIN_INPUTS);
  setMaxInputs(MAX_INPUTS);
  setMinOutputs(MIN_OUTPUTS);
  setMaxOutputs(MAX_OUTPUTS);
  setDefaultNewInputFlags(DEFAULT_NEW_INPUT_FLAGS);
  setDefaultNewOutputFlags(DEFAULT_NEW_OUTPUT_FLAGS);
  iconify(ICONIFY);
  setIconifyingHidesCentralWidget(ICONIFYING_HIDES_CENTRAL_WIDGET);

  auto add_socket = [&](Json const &a_socket, bool const a_input, uint64_t &a_socketCount) {
    auto const SOCKET_ID = a_socket["socket"].get<uint64_t>();
    auto const SOCKET_STRING_TYPE = a_socket["type"].get<std::string>();
    auto const SOCKET_NAME = a_socket["name"].get<std::string>();
    auto const SOCKET_FLAGS = a_socket["flags"].get<uint64_t>();
    auto const SOCKET_TYPE = ValueDescription::stringViewToType(SOCKET_STRING_TYPE);

    assert(a_socketCount == SOCKET_ID);

    a_input ? addInput(SOCKET_TYPE, SOCKET_NAME, SOCKET_FLAGS) : addOutput(SOCKET_TYPE, SOCKET_NAME, SOCKET_FLAGS);
    a_socketCount++;
  };

  uint64_t inputsCount{}, outputsCount{};
  for (auto &&socket : INPUTS) add_socket(socket, true, inputsCount);
  for (auto &&socket : OUTPUTS) add_socket(socket, false, outputsCount);
}

void Element::setName(std::string const &a_name)
{
  auto const OLD_NAME = m_name;
  m_name = a_name;

  handleEvent(Event{ EventType::eElementNameChanged, EventNameChanged{ OLD_NAME, a_name } });
}

bool Element::addInput(ValueType const a_type, std::string const &a_name, uint64_t const a_flags)
{
  if (m_inputs.size() + 1 > m_maxInputs) return false;

  IOSocket input{};
  input.name = a_name;
  input.type = a_type;
  input.flags = a_flags;
  if (!alwaysCalculate()) input.isMonitored = true;

  resetIOSocketValue(input);
  m_inputs.emplace_back(input);

  handleEvent(Event{ EventType::eInputAdded, EventEmpty{} });

  return true;
}

void Element::setInputName(uint64_t const a_input, std::string const &a_name)
{
  auto const OLD_NAME = m_inputs[a_input].name;
  if (OLD_NAME == a_name) return;

  m_inputs[a_input].name = a_name;

  handleEvent(Event{ EventType::eIONameChanged, EventIONameChanged{ OLD_NAME, a_name, a_input, true } });
}

void Element::removeInput()
{
  m_inputs.pop_back();

  handleEvent(Event{ EventType::eInputRemoved, EventEmpty{} });
}

void Element::clearInputs()
{
  m_inputs.clear();
}

bool Element::addOutput(ValueType const a_type, std::string const &a_name, uint64_t const a_flags)
{
  if (m_outputs.size() + 1 > m_maxOutputs) return false;

  IOSocket output{};
  output.name = a_name;
  output.type = a_type;
  output.flags = a_flags;

  resetIOSocketValue(output);
  m_outputs.emplace_back(output);

  handleEvent(Event{ EventType::eOutputAdded, EventEmpty{} });

  return true;
}

void Element::setOutputName(uint64_t const a_output, std::string const &a_name)
{
  auto const OLD_NAME = m_outputs[a_output].name;
  if (OLD_NAME == a_name) return;

  m_outputs[a_output].name = a_name;

  handleEvent(Event{ EventType::eIONameChanged, EventIONameChanged{ OLD_NAME, a_name, a_output, false } });
}

void Element::removeOutput()
{
  m_outputs.pop_back();

  handleEvent(Event{ EventType::eOutputRemoved, EventEmpty{} });
}

void Element::clearOutputs()
{
  m_outputs.clear();
}

void Element::setIOName(bool const a_input, uint64_t const a_id, std::string const &a_name)
{
  if (a_input)
    setInputName(a_id, a_name);
  else
    setOutputName(a_id, a_name);
}

void Element::setIOValueType(bool const a_input, uint64_t const a_id, ValueType const a_type)
{
  auto &io = a_input ? m_inputs[a_id] : m_outputs[a_id];
  auto const OLD_TYPE = io.type;

  if (OLD_TYPE == a_type) return;

  io.type = a_type;
  resetIOSocketValue(io);

  handleEvent(Event{ EventType::eIOTypeChanged, EventIOTypeChanged{ a_input, a_id, OLD_TYPE, a_type } });
}

bool Element::connect(size_t const a_sourceId, uint64_t const a_outputId, uint64_t const a_inputId)
{
  return m_package->connect(a_sourceId, a_outputId, m_id, a_inputId);
}

void Element::resetIOSocketValue(IOSocket &a_io)
{
  a_io.setValue(ValueDescription::defaultValue(a_io.type));
  a_io.valueChanged = true;
}

void Element::handleEvent(Event const &a_event)
{
  onEvent(a_event);
  if (m_handler) m_handler(a_event);
}

void Element::setMinInputs(uint64_t const a_min)
{
  if (a_min > m_maxInputs) return;
  m_minInputs = a_min;
}

void Element::setMaxInputs(uint64_t const a_max)
{
  if (a_max < m_minInputs) return;
  m_maxInputs = a_max;
}

void Element::setMinOutputs(uint64_t const a_min)
{
  if (a_min > m_maxOutputs) return;
  m_minOutputs = a_min;
}

void Element::setMaxOutputs(uint64_t const a_max)
{
  if (a_max < m_minOutputs) return;
  m_maxOutputs = a_max;
}

} // namespace spaghetti
