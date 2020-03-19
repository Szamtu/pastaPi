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

#include "dir_list_node.h"
#include "dir_list.h"

#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>

namespace spaghetti::nodes {

DirList::DirList() {}

void DirList::refreshCentralWidget()
{
  calculateBoundingRect();
}

void DirList::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Dir List");

  auto const CONST_DIR_LIST = static_cast<elements::DirList *>(m_element);
  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *itemSortType = new QTableWidgetItem{ "Sort Type" };
  m_properties->setItem(row, 0, itemSortType);

  QComboBox *const sortTypeComboBox = new QComboBox{};
  sortTypeComboBox->insertItems(0, { "None", "Name", "Time", "File Size", "Extension" });
  m_properties->setCellWidget(row, 1, sortTypeComboBox);

  auto const currentSortType = CONST_DIR_LIST->sortType();
  sortTypeComboBox->setCurrentIndex(currentSortType);

  QObject::connect(sortTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
                   [CONST_DIR_LIST](int const a_index) { CONST_DIR_LIST->setSortType(a_index); });

  row++;
  m_properties->insertRow(row);

  QTableWidgetItem *itemAddPath = new QTableWidgetItem{ "Add Path" };
  m_properties->setItem(row, 0, itemAddPath);

  QCheckBox *const addPathCheckBox = new QCheckBox{ "Enabled" };
  m_properties->setCellWidget(row, 1, addPathCheckBox);

  auto const currentAddPath = CONST_DIR_LIST->addPath();
  addPathCheckBox->setChecked(currentAddPath);

  QObject::connect(addPathCheckBox, &QCheckBox::stateChanged, [CONST_DIR_LIST](int const a_state) {
    if (a_state == Qt::Unchecked) {
      CONST_DIR_LIST->setAddPath(false);
    } else {
      CONST_DIR_LIST->setAddPath(true);
    }
  });
}

} // namespace spaghetti::nodes
