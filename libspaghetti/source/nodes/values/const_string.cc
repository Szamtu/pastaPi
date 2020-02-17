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

#include "nodes/values/const_string.h"
#include <spaghetti/elements/values/const_string.h>

#include <QDebug>
#include <QGraphicsSimpleTextItem>
#include <QLineEdit>
#include <QTableWidget>

namespace spaghetti::nodes::values {

ConstString::ConstString()
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

void ConstString::refreshCentralWidget()
{
  if (!m_element) return;

  std::string const VALUE{ std::get<std::string>(m_element->outputs()[0].value) };
  m_info->setText(QString(VALUE.c_str()));

  calculateBoundingRect();
}

void ConstString::showProperties()
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

  auto const CONST_STRING = static_cast<elements::values::ConstString *>(m_element);
  std::string const CURRENT = CONST_STRING->currentValue();

  QLineEdit *const value = new QLineEdit{};
  value->setText(QString::fromStdString(CURRENT));
  m_properties->setCellWidget(row, 1, value);

  QObject::connect(value, static_cast<void (QLineEdit::*)(QString const &)>(&QLineEdit::textChanged),
                   [CONST_STRING](QString a_value) { CONST_STRING->set(a_value.toStdString()); });
}

} // namespace spaghetti::nodes::values
