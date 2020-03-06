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

#ifndef PROJECTTREEWIDGET_H
#define PROJECTTREEWIDGET_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QTreeWidget>

class Editor;

class ElementsTree : public QTreeWidget {
  Q_OBJECT

 public:
  enum {
    eMetaDataType = Qt::UserRole, // type or path
    eMetaDataName = Qt::UserRole + 1,
    eMetaDataIcon = Qt::UserRole + 2,
    eMetaDataIsPackage = Qt::UserRole + 3,
    eMetaDataFilename = Qt::UserRole + 4,
  };

  explicit ElementsTree(QWidget *a_parent = nullptr);

  QTreeWidgetItem *getCathegory(QString const a_category);
  void applyFilter(QString const a_text);

 protected:
  void startDrag(Qt::DropActions a_supportedActions) override;

 private:
  QList<QPair<QTreeWidgetItem *, QPair<bool, bool>>> m_savedTreeState;
};

#endif // PROJECTTREEWIDGET_H
