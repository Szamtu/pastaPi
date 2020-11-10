// tkDNN YOLO PastaPi plugin
// Copyright (C) 2020  Pawel Adamski

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

// This plugin uses tkDNN
// Licensed under terms of GPLv2
// https://github.com/ceccocats/tkDNN

#include "dnn_tkdnn.h"
#include <chrono>

namespace spaghetti::elements {

TkDnn::TkDnn()
{
  setMinInputs(1);
  setMaxInputs(8);
  setMinOutputs(3);
  setMaxOutputs(10);

  addInput(ValueType::eMatrix, "Frame", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  setDefaultNewInputFlags(IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);

  addOutput(ValueType::eBool, "Readdy", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addOutput(ValueType::eFloat, "Inference time", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addOutput(ValueType::eDNNRectVector, "Output", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
  setDefaultNewOutputFlags(IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
}

void TkDnn::calculate()
{
  if (m_configuration.syncInputs) {
    for (auto const &input : m_inputs) {
      if (!input.valueChanged) return;
    }
  }

  if (!m_networkReaddy) return;

  std::vector<std::pair<int, int>> resultFlow{};
  for (unsigned i = 0; i < m_inputs.size() && i < m_outputs.size() + 2; i++) {
    if (m_inputs[i].valueChanged) resultFlow.push_back({ i, i + 2 });
  }

  double totalInferenceTime = 0.0;
  for (auto &flow : resultFlow) {
    auto const FROM = flow.first;
    auto const TO = flow.second;

    std::vector<cv::Mat> batch{ m_inputs[FROM].getValue<cv::Mat>() };
    if (batch.front().empty()) continue;

    auto start = std::chrono::steady_clock::now();
    m_lock.lock();
    m_yoloNetwork.update(batch);
    auto boxes = m_yoloNetwork.batchDetected;
    m_lock.unlock();
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    totalInferenceTime += elapsed.count();

    DNNRectVector resultVec;
    for (auto &box : boxes) {
      for (auto &boxElement : box) {
        DNNRect rect{ { static_cast<int>(boxElement.x), static_cast<int>(boxElement.y), static_cast<int>(boxElement.w),
                        static_cast<int>(boxElement.h) },
                      boxElement.cl,
                      boxElement.prob };
        resultVec.push_back(rect);
      }
    }

    m_outputs[TO].setValue(resultVec);
  }

  m_outputs[1].setValue<double>(totalInferenceTime);
}

void TkDnn::serialize(Json &a_json)
{
  Element::serialize(a_json);
  auto &properties = a_json["properties"];
  properties["networkFilePath"] = m_configuration.networkFilePath;
  properties["classes"] = m_configuration.classes;
  properties["confidenceThreshold"] = m_configuration.confidenceThreshold;
  properties["syncInputs"] = m_configuration.syncInputs;
}
void TkDnn::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  TkDNNConfiguration config{};
  auto const &PROPERTIES = a_json["properties"];

  config.networkFilePath = PROPERTIES["networkFilePath"].get<std::string>();
  config.classes = PROPERTIES["classes"].get<int>();
  config.confidenceThreshold = PROPERTIES["confidenceThreshold"].get<double>();
  config.syncInputs = PROPERTIES["syncInputs"].get<bool>();

  confugure(config);
}

bool TkDnn::confugure(TkDNNConfiguration const a_configuration)
{
  m_lock.lock();
  if (!m_yoloNetwork.init(a_configuration.networkFilePath, a_configuration.classes, 1,
                          a_configuration.confidenceThreshold)) {
    m_networkReaddy = false;
  } else {
    m_networkReaddy = true;
  }

  m_configuration = a_configuration;
  m_outputs[0].setValue(m_networkReaddy);
  m_lock.unlock();

  return m_networkReaddy;
}

} // namespace spaghetti::elements
