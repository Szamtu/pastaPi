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

#include "dnn_simple_tracker.h"

namespace spaghetti::elements {

using namespace std;

DnnSimpleTracker::DnnSimpleTracker()
  : Element{}
{
  setMinInputs(3);
  setMaxInputs(3);
  setMinOutputs(3);
  setMaxOutputs(3);

  addInput(ValueType::eDNNRectVector, "DNN rect vector", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Tracked class ID", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "Target Loss Aprox Max Frames", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);

  addOutput(ValueType::eInt, "Tracked objects", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addOutput(ValueType::eIntVector, "Tracked Object IDs", IOSocket::eCanHoldIntVector | IOSocket::eCanChangeName);
  addOutput(ValueType::eDNNRectVector, "Tracked objects", IOSocket::eCanHoldDNNRectVector | IOSocket::eCanChangeName);
}

void DnnSimpleTracker::calculate()
{
  auto rectVector = m_inputs[0].getValue<DNNRectVector>();
  auto classID = m_inputs[1].getValue<int>();
  auto maxLossCounter = m_inputs[2].getValue<int>();

  DNNRectVector detectedObjects{};
  for (auto const &rect : rectVector) {
    if (rect.classId == classID) detectedObjects.push_back(rect);
  }

  for (auto &trackedObject : m_trackedObjects) {
    trackedObject.targetLossCounter++;

    if (trackedObject.trackData.size() > 1) {
      auto &trackData1 = *(trackedObject.trackData.end() - 1);
      auto &trackData2 = *(trackedObject.trackData.end() - 2);

      trackedObject.nextPositionApprox.classId = trackData1.classId;
      trackedObject.nextPositionApprox.confidence = (trackData2.confidence + trackData1.confidence) / 2.0;
      if (trackedObject.nextPositionApprox.confidence > 1.0) trackedObject.nextPositionApprox.confidence = 1.0;

      auto deltaX = trackData1.boundingBox.x - trackData2.boundingBox.x;
      auto deltaY = trackData1.boundingBox.y - trackData2.boundingBox.y;
      auto deltaW = trackData1.boundingBox.width - trackData2.boundingBox.width;
      auto deltaH = trackData1.boundingBox.height - trackData2.boundingBox.height;

      trackedObject.nextPositionApprox.boundingBox =
          cv::Rect(trackData1.boundingBox.x + deltaX, trackData1.boundingBox.y + deltaY,
                   trackData1.boundingBox.width + deltaW, trackData1.boundingBox.height + deltaH);

      trackedObject.hasNextPositionApprox = true;
    }
  }

  if (detectedObjects.size() && m_trackedObjects.size()) {
    auto findClosestObject = [detectedObjects](cv::Rect const &a_a) {
      int index = 0;
      double minDistance = INT_MAX;

      for (unsigned i = 0; i < detectedObjects.size(); i++) {
        auto &a_b = detectedObjects[i].boundingBox;

        auto const midXa = a_a.x + (a_a.width / 2);
        auto const midYa = a_a.y + (a_a.height / 2);
        auto const midXb = a_b.x + (a_b.width / 2);
        auto const midYb = a_b.y + (a_b.height / 2);

        auto deltaX = std::abs(midXb - midXa);
        auto deltaY = std::abs(midYb - midYa);

        auto disance = std::sqrt(std::pow(deltaX, 2) + std::pow(deltaY, 2));

        if (minDistance > disance) {
          index = i;
          minDistance = disance;
        }
      }

      return index;
    };

    for (auto &trackedObject : m_trackedObjects) {
      if (detectedObjects.size()) {
        int newStepID;
        if (trackedObject.hasNextPositionApprox) {
          newStepID = findClosestObject(trackedObject.nextPositionApprox.boundingBox);
        } else {
          newStepID = findClosestObject(trackedObject.trackData.back().boundingBox);
        }

        trackedObject.trackData.push_back(detectedObjects[newStepID]);
        trackedObject.hasNextPositionApprox = false;
        trackedObject.targetLossCounter = 0;
        detectedObjects.erase(detectedObjects.begin() + newStepID);

      } else {
        break;
      }
    }
  }

  if (detectedObjects.size()) {
    for (auto &object : detectedObjects) {
      m_trackedObjects.push_back({ std::rand(), 0, DNNRectVector(), DNNRect(), false });
      m_trackedObjects.back().trackData.push_back(object);
    }
  }

  for (auto &tracked : m_trackedObjects) {
    if (tracked.hasNextPositionApprox) {
      tracked.hasNextPositionApprox = false;
      tracked.trackData.push_back({ tracked.nextPositionApprox });
    }

    while (tracked.trackData.size() > 2) {
      tracked.trackData.erase(tracked.trackData.begin());
    }
  }

  for (auto it = m_trackedObjects.begin(); it != m_trackedObjects.end();) {
    if ((*it).targetLossCounter > maxLossCounter) {
      it = m_trackedObjects.erase(it);
    } else {
      ++it;
    }
  }

  DNNRectVector outputTracked;
  IntVector outTrackIDs;
  for (auto const &tracked : m_trackedObjects) {
    outputTracked.push_back(tracked.trackData.back());
    outTrackIDs.push_back(tracked.trackID);
  }

  m_outputs[0].setValue(static_cast<int>(m_trackedObjects.size()));
  m_outputs[1].setValue(outTrackIDs);
  m_outputs[2].setValue(outputTracked);
}

} // namespace spaghetti::elements
