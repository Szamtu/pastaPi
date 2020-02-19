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

#include <spaghetti/custom_value_types/all.h>
#include <QColor>
#include <QString>
#include <variant>

namespace spaghetti {
enum class ValueType { eBool, eInt, eFloat, eString, eMatrix };

using Value = std::variant<bool, int32_t, float, std::string, Matrix>;

struct IOSocketFlags {
  enum Flags {
    eCanHoldBool = 1 << 0,
    eCanHoldInt = 1 << 1,
    eCanHoldFloat = 1 << 2,
    eCanHoldString = 1 << 3,
    eCanHoldMatrix = 1 << 4,
    eCanChangeName = 1 << 5,
    eCanHoldAllValues = eCanHoldBool | eCanHoldInt | eCanHoldFloat | eCanHoldString,
    eDefaultFlags = eCanHoldAllValues | eCanChangeName
  };
};

QString ValueType_to_QString(ValueType const a_type);
const char *getSocketType(ValueType const a_type);
bool value_type_allowed(uint8_t const a_flags, ValueType const a_type);
ValueType first_available_type_for_flags(uint8_t const a_flags);
std::pair<QColor, QColor> getTypeColor(ValueType const a_type);
ValueType stringViewToType(std::string_view const a_type);
std::vector<ValueType> getHoldedValues(IOSocketFlags::Flags const a_flags);
Value getDefaultValue(ValueType const a_valueType);

} // namespace spaghetti

#endif // SPAGHETTI_SOCKVALUES_H
