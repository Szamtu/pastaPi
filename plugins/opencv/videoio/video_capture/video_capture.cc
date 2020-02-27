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

#include "video_capture.h"

namespace spaghetti::elements {
VideoCapture::VideoCapture()
  : Element{}
{
  setMinInputs(1);
  setMaxInputs(1);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eString, "Source", IOSocket::eCanHoldString | IOSocket::eCanChangeName);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addOutput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void VideoCapture::calculate()
{
  auto source = m_inputs[0].getValue<std::string>();
  if (source != m_sourceStr) {
    m_cap.release();
    m_sourceStr = source;
    if (!m_sourceStr.empty()) m_cap.open(m_sourceStr);
  }

  if (m_cap.isOpened()) {
    m_outputs[0].setValue(true);
    if (m_cap.hasNewFrame()) {
      m_outputs[1].setValue(m_cap.grabFrame());
    }

  } else {
    m_outputs[0].setValue(false);
    if (!m_sourceStr.empty()) m_cap.open(m_sourceStr);
  }
}

} // namespace spaghetti::elements

CapAsync::CapAsync() {}

CapAsync::~CapAsync()
{
  release();
}

void CapAsync::release()
{
  if (m_isOpened) {
    m_killThread = true;

    if (m_captureThread.joinable()) m_captureThread.join();
    m_cap.release();

    m_killThread = false;
  }
  m_isOpened = false;
}

bool CapAsync::open(std::string const a_name)
{
  if (!m_isOpened) {
    m_cap.open(a_name);
    if (m_cap.isOpened()) {
      m_isOpened = true;
      m_killThread = false;
      m_captureThread = std::thread(capture, this);
      return true;
    }
  }

  return false;
}

bool CapAsync::isOpened()
{
  return m_isOpened && m_captureThread.joinable();
}

bool CapAsync::hasNewFrame()
{
  return m_hasNewFrame;
}

cv::Mat CapAsync::grabFrame()
{
  m_hasNewFrame = false;
  return m_frame.clone();
}

void CapAsync::capture(CapAsync *a_context)
{
  while (!a_context->m_killThread) {
    cv::Mat image{};
    if (a_context->m_cap.isOpened()) {
      a_context->m_cap.read(image);
      if (!a_context->m_hasNewFrame) {
        a_context->m_frame = image.clone();
        a_context->m_hasNewFrame = true;
      }
    } else {
      return;
    }
  }
}
