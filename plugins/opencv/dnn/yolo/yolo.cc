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

#include "yolo.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <istream>
#include <opencv2/core/cuda.hpp>

namespace spaghetti::elements {

using namespace std;

Yolo::Yolo()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(3);
  setMaxOutputs(3);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addOutput(ValueType::eDNNData, "Output", IOSocket::eCanHoldDNNData | IOSocket::eCanChangeName);
  addOutput(ValueType::eFloat, "ComputeTime [s]", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addOutput(ValueType::eBool, "Readdy", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void Yolo::calculate()
{
  auto frame = m_inputs[0].getValue<cv::Mat>();

  if (m_readdyToUse && !frame.empty()) {
    auto start = std::chrono::steady_clock::now();

    cv::Mat blob;
    DNNData output;

    cv::dnn::blobFromImage(frame, blob, 1 / 255.0, cv::Size(m_inputWidth, m_inputHeight), cv::Scalar(0, 0, 0), true,
                           false);
    m_net.setInput(blob);
    m_net.forward(output, m_outputsNames);
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    m_outputs[0].setValue(output);
    m_outputs[1].setValue(elapsed.count());
  }

  m_outputs[2].setValue(m_readdyToUse);
}

void Yolo::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];
  properties["modelConfigurationPath"] = m_yoloConfiguration.modelConfigurationPath;
  properties["weightsPath"] = m_yoloConfiguration.weightsFilePath;
  properties["cudaEnabled"] = m_yoloConfiguration.cudaEnabled;
}

void Yolo::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  YoloConfiguration yoloConfig{};
  auto const &PROPERTIES = a_json["properties"];

  yoloConfig.modelConfigurationPath = PROPERTIES["modelConfigurationPath"].get<std::string>();
  yoloConfig.weightsFilePath = PROPERTIES["weightsPath"].get<std::string>();
  yoloConfig.cudaEnabled = PROPERTIES["cudaEnabled"].get<bool>();

  confugure(yoloConfig);
}

bool Yolo::confugure(YoloConfiguration const a_configuration)
{
  m_readdyToUse = false;
  m_yoloConfiguration = a_configuration;
  m_outputs[2].setValue(m_readdyToUse);

  try {
    auto net = cv::dnn::readNetFromDarknet(a_configuration.modelConfigurationPath, a_configuration.weightsFilePath);

    if (a_configuration.cudaEnabled) {
      net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
      net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    } else {
      net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
      net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }

    if (!loadInputSizes(a_configuration.modelConfigurationPath))
      CV_Error(cv::Error::StsError, "Cannot load inpus size from configuration file!");
    m_outputsNames = getOutputsNames(net);

    m_net = net;
    m_readdyToUse = true;
    m_outputs[2].setValue(m_readdyToUse);

    return true;
  } catch (cv::Exception error) {
    m_lastError = error.msg;
  }

  return false;
}

std::string Yolo::lastError()
{
  auto error = m_lastError;
  m_lastError = "";
  return error;
}

bool Yolo::isCudaAvailable()
{
  if (cv::cuda::getCudaEnabledDeviceCount() > 0) {
    return true;
  }
  return false;
}

bool Yolo::loadInputSizes(std::string const a_path)
{
  std::ifstream classesFileStream(a_path);
  std::string line{};

  if (!classesFileStream.is_open()) return false;

  bool inputWidthIsSet{};
  bool inputHeightIsSet{};

  while (getline(classesFileStream, line)) {
    auto separatorPos = line.find('=');

    if (separatorPos == std::string::npos || separatorPos + 1 >= line.size()) continue;

    auto paramStr = line.substr(0, separatorPos);
    auto valueStr = line.substr(separatorPos + 1);

    if (paramStr == "width") {
      m_inputWidth = std::atoi(valueStr.c_str());
      inputWidthIsSet = true;
    } else if (paramStr == "height") {
      m_inputHeight = std::atoi(valueStr.c_str());
      inputHeightIsSet = true;
    }
  }

  if (!inputWidthIsSet || !inputHeightIsSet) return false;

  return true;
}

std::vector<std::string> Yolo::getOutputsNames(cv::dnn::Net const &a_net)
{
  std::vector<std::string> names{};
  auto outLayers = a_net.getUnconnectedOutLayers();
  auto layersNames = a_net.getLayerNames();

  names.resize(outLayers.size());
  for (size_t i = 0; i < outLayers.size(); ++i) {
    names[i] = layersNames[static_cast<size_t>(outLayers[i]) - 1];
  }

  return names;
}

} // namespace spaghetti::elements
