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
#ifndef SPAGHETTI_EDITOR_PACKAGE_H
#define SPAGHETTI_EDITOR_PACKAGE_H

class QString;
class QTableWidget;
class QListView;

namespace spaghetti {

class Package;
class PackageView;

class EditorPackage {
 public:
  virtual ~EditorPackage() {}
  virtual void openOrCreatePackageView(Package *const a_package) = 0;
  virtual void setPackageViewTabName(int const a_index, QString const &a_name) = 0;
  virtual int indexForPackageView(PackageView *const a_packageView) const = 0;
  virtual QTableWidget *propertiesTable() = 0;
  virtual QListView *elementsList() = 0;
};
} // namespace spaghetti

#endif // SPAGHETTI_EDITOR_PACKAGE_H
