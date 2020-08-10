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

#include "video_writer.h"
#include <spaghetti/utils.h>

namespace spaghetti::elements {
VideoWriter::VideoWriter()
  : Element{}
{
  setMinInputs(7);
  setMaxInputs(7);
  setMinOutputs(1);
  setMaxOutputs(1);

  addInput(ValueType::eMatrix, "Image", IOSocket::eCanHoldMatrix | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "Write", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
  addInput(ValueType::eString, "Destination", IOSocket::eCanHoldString | IOSocket::eCanChangeName);
  addInput(ValueType::eFloat, "FrameRate", IOSocket::eCanHoldFloat | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "ResWidth", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eInt, "ResHeight", IOSocket::eCanHoldInt | IOSocket::eCanChangeName);
  addInput(ValueType::eBool, "Color", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);

  addOutput(ValueType::eBool, "Writing", IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void VideoWriter::calculate()
{
  auto enabled = m_inputs[1].getValue<bool>();
  if (enabled) {
    auto destination = m_inputs[2].getValue<std::string>();
    auto frameRate = m_inputs[3].getValue<double>();
    auto resWidth = m_inputs[4].getValue<int>();
    auto resHeight = m_inputs[5].getValue<int>();
    auto color = m_inputs[6].getValue<bool>();

    bool reopen{};
    if (m_destination != destination) {
      m_destination = destination;
      reopen = true;
    }

    if (!nearly_equal(m_framerate, frameRate)) {
      m_framerate = frameRate;
      reopen = true;
    }

    if (m_resWidth != resWidth) {
      m_resWidth = resWidth;
      reopen = true;
    }

    if (m_resHeight != resHeight) {
      m_resHeight = resHeight;
      reopen = true;
    }

    if (m_color != color) {
      m_color = color;
      reopen = true;
    }

    if (reopen) {
      if (m_writer.isOpened()) m_writer.release();
    }

    auto failure = false;
    if (m_resWidth < 1 || m_resHeight < 1 || m_framerate < 0.1 || m_destination.empty()) {
      failure = true;
    }

    if (!m_writer.isOpened() && !failure) {
      m_writer.open(m_destination, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), m_framerate,
                    cv::Size(m_resWidth, m_resHeight), m_color);
    }

    if (m_writer.isOpened()) {
      auto matrix = m_inputs[0].getValue<cv::Mat>();
      m_writer.saveFrame(matrix);
    }

  } else {
    if (m_writer.isOpened()) m_writer.release();
  }

  m_outputs[0].setValue(m_writer.isOpened());
}

} // namespace spaghetti::elements

WriterAsync::WriterAsync() {}
WriterAsync::~WriterAsync()
{
  release();
}

void WriterAsync::release()
{
  if (m_isOpened) {
    m_killThread = true;

    if (m_writerThread.joinable()) m_writerThread.join();
    m_writer.release();

    m_killThread = false;

    for (auto &frame : m_frames) frame.release();
    m_frames.clear();
  }
  m_isOpened = false;
}

bool WriterAsync::open(std::string const a_name, int const a_fourcc, double const a_fps, cv::Size const a_resolution,
                       bool const a_color)
{
  if (!m_isOpened) {
    m_writer.open(a_name, a_fourcc, a_fps, a_resolution, a_color);
    if (m_writer.isOpened()) {
      m_isOpened = true;
      m_killThread = false;
      m_writerThread = std::thread(write, this);

      return true;
    }
  }

  return false;
}

void WriterAsync::saveFrame(cv::Mat const a_frame)
{
  const std::lock_guard<std::mutex> lock(m_mutex);
  m_frames.push_back(a_frame);
}

void WriterAsync::write(WriterAsync *a_context)
{
  while (!a_context->m_killThread && a_context->m_writer.isOpened()) {
    if (!a_context->m_frames.empty()) {
      a_context->m_mutex.lock();
      auto frame = a_context->m_frames.front().clone();
      a_context->m_frames.pop_front();
      a_context->m_mutex.unlock();

      a_context->m_writer << frame;
    }
  }
}
