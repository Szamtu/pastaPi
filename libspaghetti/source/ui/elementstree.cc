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

  auto baseCategory = new QTreeWidgetItem{};
  baseCategory->setText(0, "Base");
  addTopLevelItem(baseCategory);
  baseCategory->setHidden(true);
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

  auto CreateCategory = [](QString const a_text, QTreeWidgetItem *a_parent = nullptr) -> auto
  {
    auto item = new QTreeWidgetItem{ a_parent };
    item->setText(0, a_text);
    auto font = item->font(0);
    font.setBold(true);
    item->setFont(0, font);

    return item;
  };

  auto toplevelCategories = findItems(categoryList[0], Qt::MatchExactly);
  if (!toplevelCategories.size()) {
    categoryItem = CreateCategory(categoryList[0]);
    addTopLevelItem(categoryItem);
  } else {
    categoryItem = toplevelCategories[0];
  }
  categoryList.removeFirst();

  for (auto &subCategory : categoryList) {
    if (!categoryItem->childCount()) {
      categoryItem = CreateCategory(subCategory, categoryItem);
    } else {
      QTreeWidgetItem *subCategoryItem{ nullptr };
      for (int childNum = 0; childNum < categoryItem->childCount(); childNum++) {
        if (categoryItem->child(childNum)->text(0) == subCategory) {
          subCategoryItem = categoryItem->child(childNum);
          break;
        }
      }

      if (subCategoryItem) {
        categoryItem = subCategoryItem;
      } else {
        categoryItem = CreateCategory(subCategory, categoryItem);
      }
    }
  }

  return categoryItem;
}

void ElementsTree::applyFilter(QString const a_text)
{
  bool saveState = m_savedTreeState.empty();

  auto allItems = findItems("", Qt::MatchRecursive | Qt::MatchContains);
  for (auto &element : allItems) {
    if (saveState) m_savedTreeState.push_back({ element, { element->isHidden(), element->isExpanded() } });
    element->setExpanded(true);
    element->setHidden(true);
  }

  if (!a_text.isEmpty()) {
    auto foundedItems = findItems(a_text, Qt::MatchRecursive | Qt::MatchContains);
    for (auto &item : foundedItems) {
      if (item->text(0) == "Base" || item->text(0) == "Package") continue;

      auto parent = item->parent();
      while (parent) {
        parent->setHidden(false);
        parent = parent->parent();
      }

      item->setHidden(false);
    }
  } else {
    if (!m_savedTreeState.empty()) {
      for (auto const &item : m_savedTreeState) {
        item.first->setHidden(item.second.first);
        item.first->setExpanded(item.second.second);
      }
      m_savedTreeState.clear();
    }
  }
}
