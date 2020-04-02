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
#ifndef SPAGHETTI_SOCKVALUES_H
#define SPAGHETTI_SOCKVALUES_H

#include <spaghetti/api.h>
#include <opencv2/core/mat.hpp>
#include <variant>

namespace spaghetti {
enum class ValueType {
  eBool,
  eInt,
  eFloat,
  eFloatVector,
  eString,
  eStringVector,
  eMatrix,
  eMatrixVector,
  ePoint,
  eShape,
  eShapeVector,
  eDNNData,
  eDNNRect,
  eDNNRectVector
};

using FloatVector = std::vector<double>;
using MatrixVector = std::vector<cv::Mat>;
using Shape = std::vector<cv::Point>;
using ShapeVector = std::vector<std::vector<cv::Point>>;
using StringVector = std::vector<std::string>;
using DNNData = std::vector<cv::Mat>;

typedef struct DNNRect {
  cv::Rect boundingBox{};
  int classId{};
  float confidence{};
} DNNRect;

using DNNRectVector = std::vector<DNNRect>;

using Value = std::variant<bool, int32_t, double, FloatVector, std::string, StringVector, cv::Mat, cv::Point, Shape,
                           ShapeVector, DNNData, DNNRect, DNNRectVector>;

struct IOSocketFlags {
  enum Flags {
    eCanHoldBool = 1 << 0,
    eCanHoldInt = 1 << 1,
    eCanHoldFloat = 1 << 2,
    eCanHoldFloatVector = 1 << 3,
    eCanHoldString = 1 << 4,
    eCanHoldStringVector = 1 << 5,
    eCanHoldMatrix = 1 << 6,
    eCanHoldMatrixVector = 1 << 7,
    eCanHoldPoint = 1 << 8,
    eCanHoldShape = 1 << 9,
    eCanHoldShapeVector = 1 << 10,
    eCanHoldDNNData = 1 << 11,
    eCanHoldDNNRect = 1 << 12,
    eCanHoldDNNRectVector = 1 << 13,
    eCanChangeName = 1 << 14,
    eCanHoldAllValues = eCanHoldBool | eCanHoldInt | eCanHoldFloat | eCanHoldFloatVector | eCanHoldString |
                        eCanHoldStringVector | eCanHoldMatrix | eCanHoldMatrixVector | eCanHoldPoint | eCanHoldShape |
                        eCanHoldShapeVector | eCanHoldDNNData | eCanHoldDNNRect | eCanHoldDNNRectVector,
    eDefaultFlags = eCanHoldAllValues | eCanChangeName,
    eProtectedValuesFlags = eCanHoldFloatVector | eCanHoldStringVector | eCanHoldMatrix | eCanHoldMatrixVector |
                            eCanHoldShape | eCanHoldShapeVector | eCanHoldDNNData | eCanHoldDNNRect |
                            eCanHoldDNNRectVector,
    eTimeStampedValues = eProtectedValuesFlags
  };
};

struct ElementColor {
  int r{};
  int g{};
  int b{};
  int a{};
};

struct ValueTypeDescription {
  ValueTypeDescription(ValueType const a_type, IOSocketFlags::Flags const a_holdType, std::string const a_typeString,
                       std::string const a_typeQstring, std::pair<ElementColor, ElementColor> const a_colors,
                       Value const a_value);
  ValueType const type{};
  IOSocketFlags::Flags const holdType{};
  std::string const typeString{};
  std::string const typeQstring{};
  std::pair<ElementColor, ElementColor> const colors{};
  Value const value{};
};

class SPAGHETTI_API ValueDescription {
 public:
  static std::string typeStdString(ValueType const a_type);
  static const char *typeCString(ValueType const a_type);
  static bool isTypeAlowed(ValueType const a_type, IOSocketFlags::Flags const a_flags);
  static ValueType firstAvailableTypeForFlags(IOSocketFlags::Flags const a_flags);
  static std::pair<ElementColor, ElementColor> typeColors(ValueType const a_type);
  static ValueType stringViewToType(std::string_view const a_type);
  static std::vector<ValueType> heldedValues(IOSocketFlags::Flags const a_flags);
  static Value defaultValue(ValueType const a_valueType);
  static bool compareValues(Value const &a_a, Value const &a_b, ValueType const a_type);
};

} // namespace spaghetti

#endif // SPAGHETTI_SOCKVALUES_H
