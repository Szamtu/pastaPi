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

#pragma once
#ifndef SPAGHETTI_ELEMENTS_OPENCV_YOLO_H
#define SPAGHETTI_ELEMENTS_OPENCV_YOLO_H

#include <spaghetti/element.h>
#include <opencv2/dnn.hpp>

namespace spaghetti::elements {
class Yolo final : public Element {
 public:
  static constexpr char const *const TYPE{ "OpenCV/DNN/yolov3" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };
  bool alwaysCalculate() const noexcept override { return false; }

  Yolo();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  struct YoloConfiguration {
    std::string modelConfigurationPath{};
    std::string weightsFilePath{};
    std::string classesFilePath{};
    bool cudaEnabled{};
  };

  bool confugure(YoloConfiguration const a_configuration);
  std::string lastError();
  YoloConfiguration configuration() { return m_yoloConfiguration; }

  bool isCudaAvailable();

 private:
  YoloConfiguration m_yoloConfiguration{};
  std::vector<std::string> m_outputsNames{};
  int m_inputWidth{};
  int m_inputHeight{};

  std::string m_lastError{};
  bool m_readdyToUse{};

  cv::dnn::Net m_net{};

  bool loadInputSizes(std::string const a_path);
  std::vector<std::string> getOutputsNames(cv::dnn::Net const &a_net);
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS_OPENCV_YOLO_H
