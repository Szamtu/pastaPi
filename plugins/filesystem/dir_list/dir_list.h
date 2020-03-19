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
#ifndef SPAGHETTI_ELEMENTS_FILESYSTEM_DIR_LIST_H
#define SPAGHETTI_ELEMENTS_FILESYSTEM_DIR_LIST_H

#include <spaghetti/element.h>

namespace spaghetti::elements {

class DirList final : public Element {
 public:
  static constexpr char const *const TYPE{ "Filesystem/dir_list" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };
  bool alwaysCalculate() const noexcept override { return false; }

  DirList();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  void setSortType(int const a_sortType);
  void setAddPath(bool const a_enabled);

  int sortType();
  bool addPath();

 private:
  int m_sortType{};
  bool m_addPath{};
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS
