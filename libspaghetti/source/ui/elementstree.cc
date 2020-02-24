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

#include "elementstree.h"
#include "package_view.h"
#include "spaghetti/editor.h"

#include <QDebug>
#include <QDrag>
#include <QDropEvent>
#include <QMimeData>

ElementsTree::ElementsTree(QWidget *a_parent)
  : QTreeWidget{ a_parent }
{
  assert(a_parent);

  setIconSize(QSize{ 32, 32 });
}

void ElementsTree::startDrag(Qt::DropActions a_supportedActions)
{
  (void)a_supportedActions;
  if (currentItem()->childCount()) return;

  auto const item = currentItem();
  auto const TYPE = item->data(0, ElementsTree::eMetaDataType).toString();
  auto const IS_PACKAGE = item->data(0, ElementsTree::eMetaDataIsPackage).toByteArray();
  auto const NAME = item->data(0, ElementsTree::eMetaDataName).toByteArray();
  auto const ICON = item->data(0, ElementsTree::eMetaDataIcon).toByteArray();
  auto const FILE = item->data(0, ElementsTree::eMetaDataFilename).toByteArray();

  auto const mimeData = new QMimeData;
  mimeData->setText(TYPE);
  mimeData->setData("metadata/is_package", IS_PACKAGE);
  mimeData->setData("metadata/name", NAME);
  mimeData->setData("metadata/icon", ICON);
  mimeData->setData("metadata/filename", FILE);

  auto const drag = new QDrag{ this };
  drag->setMimeData(mimeData);
  drag->exec(Qt::CopyAction);
}

QTreeWidgetItem *ElementsTree::getCathegory(QString const a_category)
{
  auto categoryList = a_category.split('/');
  QTreeWidgetItem *categoryItem{ nullptr };

  for (auto category : categoryList) {
    auto categorySearchResult = findItems(category, Qt::MatchExactly);

    if (categorySearchResult.size()) {
      categoryItem = categorySearchResult.first();
    } else {
      if (categoryItem) {
        categoryItem = new QTreeWidgetItem{ categoryItem };
      } else {
        categoryItem = new QTreeWidgetItem{};
        addTopLevelItem(categoryItem);
      }
      categoryItem->setText(0, category);
    }
  }

  return categoryItem;
}
