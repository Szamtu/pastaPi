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

#include "dir_list.h"
#include <QDir>

namespace spaghetti::elements {

DirList::DirList()
  : Element{}
{
  setMinInputs(2);
  setMaxInputs(2);
  setMinOutputs(2);
  setMaxOutputs(2);

  addInput(ValueType::eString, "Dir", IOSocket::eCanHoldString | IOSocket::eCanChangeName);
  addInput(ValueType::eString, "Filter", IOSocket::eCanHoldString | IOSocket::eCanChangeName);

  addOutput(ValueType::eStringVector, "Content", IOSocket::eCanHoldStringVector | IOSocket::eCanChangeName);

  setDefaultNewInputFlags(IOSocket::eCanHoldBool | IOSocket::eCanChangeName);
}

void DirList::calculate()
{
  auto const dir = QString::fromStdString(m_inputs[0].getValue<std::string>());
  auto const filter = QString::fromStdString(m_inputs[1].getValue<std::string>());

  QDir directory{ dir, filter };

  if (m_sortType > 0) {
    directory.setSorting(static_cast<QDir::SortFlag>(m_sortType - 1));
  }

  auto const entryList = directory.entryList();
  StringVector fileList{};

  for (auto const &entry : entryList) {
    if (m_addPath) {
      auto fullFileUrl = directory.filePath(entry);
      fileList.push_back(fullFileUrl.toStdString());
    } else {
      fileList.push_back(entry.toStdString());
    }
  }

  m_outputs[0].setValue<StringVector>(fileList);
}

void DirList::serialize(Json &a_json)
{
  Element::serialize(a_json);

  auto &properties = a_json["properties"];

  properties["sortType"] = m_sortType;
  properties["addPath"] = m_addPath;
}

void DirList::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);

  auto const &PROPERTIES = a_json["properties"];
  m_sortType = PROPERTIES["sortType"].get<int>();
  m_addPath = PROPERTIES["addPath"].get<bool>();
}

void DirList::setSortType(int const a_sortType)
{
  m_sortType = a_sortType;
  calculate();
}

void DirList::setAddPath(bool const a_enabled)
{
  m_addPath = a_enabled;
  calculate();
}

int DirList::sortType()
{
  return m_sortType;
}

bool DirList::addPath()
{
  return m_addPath;
}

} // namespace spaghetti::elements
