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

#include <QColor>
#include <QString>
#include <opencv2/core/mat.hpp>
#include <variant>

namespace spaghetti {
enum class ValueType { eBool, eInt, eFloat, eString, eMatrix, ePoint, eShape, eShapeVector };

using Shape = std::vector<cv::Point>;
using ShapeVector = std::vector<std::vector<cv::Point>>;

using Value = std::variant<bool, int32_t, double, std::string, cv::Mat, cv::Point, Shape, ShapeVector>;

struct IOSocketFlags {
  enum Flags {
    eCanHoldBool = 1 << 0,
    eCanHoldInt = 1 << 1,
    eCanHoldFloat = 1 << 2,
    eCanHoldString = 1 << 3,
    eCanHoldMatrix = 1 << 4,
    eCanHoldPoint = 1 << 5,
    eCanHoldShape = 1 << 6,
    eCanHoldShapeVector = 1 << 7,
    eCanChangeName = 1 << 8,
    eCanHoldAllValues = eCanHoldBool | eCanHoldInt | eCanHoldFloat | eCanHoldString | eCanHoldMatrix | eCanHoldPoint |
                        eCanHoldShape | eCanHoldShapeVector,
    eDefaultFlags = eCanHoldAllValues | eCanChangeName,
    eProtectedValuesFlags = eCanHoldMatrix | eCanHoldShape | eCanHoldShapeVector
  };
};

struct ValueTypeDescription {
  ValueTypeDescription(ValueType const a_type, IOSocketFlags::Flags const a_holdType, std::string const a_typeString,
                       QString const a_typeQstring, std::pair<QColor, QColor> const a_colors, Value const a_value);
  ValueType const type{};
  IOSocketFlags::Flags const holdType{};
  std::string const typeString{};
  QString const typeQstring{};
  std::pair<QColor, QColor> const colors{};
  Value const value{};
};

class ValueDescription {
 public:
  static QString typeQstring(ValueType const a_type);
  static const char *typeCString(ValueType const a_type);
  static bool isTypeAlowed(ValueType const a_type, IOSocketFlags::Flags const a_flags);
  static ValueType firstAvailableTypeForFlags(IOSocketFlags::Flags const a_flags);
  static std::pair<QColor, QColor> typeColors(ValueType const a_type);
  static ValueType stringViewToType(std::string_view const a_type);
  static std::vector<ValueType> heldedValues(IOSocketFlags::Flags const a_flags);
  static Value defaultValue(ValueType const a_valueType);
};

} // namespace spaghetti

#endif // SPAGHETTI_SOCKVALUES_H
