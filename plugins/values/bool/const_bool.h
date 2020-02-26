// MIT License
//
// Copyright (c) 2017-2018 Artur Wyszyński, aljen at hitomi dot pl
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
#ifndef SPAGHETTI_ELEMENTS_VALUES_CONST_BOOL_H
#define SPAGHETTI_ELEMENTS_VALUES_CONST_BOOL_H

#include <spaghetti/element.h>

namespace spaghetti::elements {

class ConstBool final : public Element {
 public:
  static constexpr char const *const TYPE{ "Values/Bool/bool" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };

  ConstBool();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void toggle();
  void set(bool a_state);

  bool currentValue() const { return m_currentValue; }

 private:
  bool m_currentValue{};
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS_VALUES_CONST_BOOL_H
