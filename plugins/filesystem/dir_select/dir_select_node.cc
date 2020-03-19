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

#include "dir_select_node.h"
#include "dir_select.h"

#include <QFileDialog>
#include <QGraphicsSimpleTextItem>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>

namespace spaghetti::nodes {

DirSelect::DirSelect()
{
  QFont font{};
  font.setFamily("Consolas");
  font.setPointSize(10);
  auto widget = new QGraphicsSimpleTextItem();
  widget->setFont(font);

  auto brush = widget->brush();
  brush.setColor(Qt::white);
  widget->setBrush(brush);

  setCentralWidget(widget);

  m_info = widget;
}

void DirSelect::refreshCentralWidget()
{
  if (!m_element) return;

  std::string const VALUE{ m_element->outputs()[0].getValue<std::string>() };
  m_info->setText(QString(VALUE.c_str()));

  calculateBoundingRect();
}

void DirSelect::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eOutputs);

  propertiesInsertTitle("Const String");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *item{};

  item = new QTableWidgetItem{ "Value" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  QPushButton *selectButton = new QPushButton{ "Select dir" };
  m_properties->setCellWidget(row, 1, selectButton);

  auto const CONST_DIR = static_cast<elements::DirSelect *>(m_element);
  QObject::connect(selectButton, &QPushButton::clicked, [CONST_DIR]() {
    auto const currentDir = QString::fromStdString(CONST_DIR->currentValue());
    QString dir = QFileDialog::getExistingDirectory(nullptr, "Open Directory", currentDir,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    CONST_DIR->set(dir.toStdString());
  });
}

} // namespace spaghetti::nodes
