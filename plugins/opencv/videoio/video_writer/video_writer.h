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
#ifndef SPAGHETTI_ELEMENTS_OPENCV_WRITER_H
#define SPAGHETTI_ELEMENTS_OPENCV_WRITER_H

#include <spaghetti/element.h>
#include <list>
#include <opencv2/videoio.hpp>

class WriterAsync {
 public:
  WriterAsync();
  ~WriterAsync();

  void release();
  bool open(std::string const a_name, int const a_fourcc, double const a_fps, cv::Size const a_resolution,
            bool const a_color);
  bool isOpened() { return m_isOpened; }
  void saveFrame(cv::Mat const a_frame);

 private:
  std::thread m_writerThread{};
  bool m_isOpened{};
  bool m_killThread{};
  cv::VideoWriter m_writer{};

  std::mutex m_mutex;
  std::list<cv::Mat> m_frames{};

  static void write(WriterAsync *a_context);
};

namespace spaghetti::elements {
class VideoWriter final : public Element {
 public:
  static constexpr char const *const TYPE{ "OpenCV/VideoIO/video_writer" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  VideoWriter();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

 private:
  std::string m_destination{};
  double m_framerate{};
  int m_resWidth{};
  int m_resHeight{};
  bool m_color{};

  WriterAsync m_writer{};
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS_OPENCV_WRITER_H
