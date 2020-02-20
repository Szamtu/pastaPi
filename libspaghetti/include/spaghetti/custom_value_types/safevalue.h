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
#ifndef SPAGHETTI_CUSTOM_VALUE_SAFE_VALUE
#define SPAGHETTI_CUSTOM_VALUE_SAFE_VALUE

#include <QColor>
#include <QString>
#include <chrono>
#include <mutex>
#include <opencv4/opencv2/core/mat.hpp>
#include <variant>

namespace spaghetti {

using SafeValueTimeStamp = std::chrono::high_resolution_clock::time_point;
template<class T>
class SafeValue {
 public:
  SafeValue() {}
  SafeValue(T const a_data)
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_data = a_data;
    m_timeStamp = std::chrono::high_resolution_clock::now();
  }

  SafeValue(SafeValue const &a_value)
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_data = a_value.m_data;
    m_timeStamp = a_value.m_timeStamp;
  }

  SafeValue operator=(SafeValue const &a_value)
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_data = a_value.m_data;
    m_timeStamp = a_value.m_timeStamp;
    return *this;
  }

  ~SafeValue() { const std::lock_guard<std::mutex> lock(m_mutex); }

  T data()
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_data;
  }
  SafeValueTimeStamp timeStamp() { return m_timeStamp; }

 private:
  std::mutex m_mutex;
  T m_data{};
  SafeValueTimeStamp m_timeStamp;
};

} // namespace spaghetti

#endif // SPAGHETTI_CUSTOM_VALUE_SAFE_VALUE
