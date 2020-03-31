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

#include "dnn_threshold.h"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

namespace spaghetti::elements {

using namespace std;

DnnThreshold::DnnThreshold()
  : Element{}
{
  setMinInputs(4);
  setMaxInputs(4);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eDNNData, "DNNData", IOSocket::eCanHoldDNNData | IOSocket::eCanChangeName);
  addInput(ValueType::eMatrix, "Frame", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "Confidence threshold", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "NMS threshold", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eDNNRectVector, "DNN rects", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
}

void DnnThreshold::calculate()
{
  auto dnnData = m_inputs[0].getValue<DNNData>();
  auto frame = m_inputs[1].getValue<cv::Mat>();
  auto confidenceThreshold = m_inputs[2].getValue<double>();
  auto nmsThreshold = m_inputs[3].getValue<double>();

  if (dnnData.size() && frame.size) {
    vector<int> classIds;
    vector<float> confidences;
    vector<cv::Rect> boxes;

    for (size_t i = 0; i < dnnData.size(); ++i) {
      auto data = reinterpret_cast<float *>(dnnData[i].data);
      for (int j = 0; j < dnnData[i].rows; ++j, data += dnnData[i].cols) {
        cv::Mat scores = dnnData[i].row(j).colRange(5, dnnData[i].cols);

        cv::Point classIdPoint;
        double confidence;

        minMaxLoc(scores, nullptr, &confidence, nullptr, &classIdPoint);
        if (confidence > confidenceThreshold) {
          int centerX = static_cast<int>((data[0] * frame.cols));
          int centerY = static_cast<int>((data[1] * frame.rows));
          int width = static_cast<int>((data[2] * frame.cols));
          int height = static_cast<int>((data[3] * frame.rows));
          int left = centerX - width / 2;
          int top = centerY - height / 2;

          classIds.push_back(classIdPoint.x);
          confidences.push_back(static_cast<float>(confidence));
          boxes.push_back(cv::Rect(left, top, width, height));
        }
      }
    }

    DNNRectVector output{};
    vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, static_cast<float>(confidenceThreshold), static_cast<float>(nmsThreshold),
                      indices);
    for (size_t i = 0; i < indices.size(); ++i) {
      auto idx = static_cast<size_t>(indices[i]);
      output.push_back({ boxes[idx], classIds[idx], confidences[idx] });
    }

    m_outputs[0].setValue(output);
  }
}

} // namespace spaghetti::elements
