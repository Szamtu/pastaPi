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
#ifndef SPAGHETTI_CUSTOM_VALUE_MATRIX
#define SPAGHETTI_CUSTOM_VALUE_MATRIX

#include <QColor>
#include <QString>
#include <chrono>
#include <mutex>
#include <opencv4/opencv2/core/mat.hpp>
#include <variant>

namespace spaghetti {
using MatrixTimeStamp = std::chrono::high_resolution_clock::time_point;

class Matrix {
 public:
  Matrix() {}
  Matrix(cv::Mat const a_mat)
  {
    m_mat = a_mat;
    m_timeStamp = std::chrono::high_resolution_clock::now();
  }
  Matrix(const Matrix &a_mat)
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_mat = a_mat.m_mat;
    m_timeStamp = a_mat.m_timeStamp;
  }

  Matrix operator=(const Matrix &a_mat)
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    m_mat = a_mat.m_mat;
    m_timeStamp = a_mat.m_timeStamp;
    return *this;
  }

  ~Matrix() { const std::lock_guard<std::mutex> lock(m_mutex); }

  cv::Mat cvMat()
  {
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_mat;
  }

  MatrixTimeStamp timeStamp() const { return m_timeStamp; }

 private:
  std::mutex m_mutex;
  cv::Mat m_mat{};
  MatrixTimeStamp m_timeStamp;
};
} // namespace spaghetti

#endif // SPAGHETTI_CUSTOM_VALUE_MATRIX
