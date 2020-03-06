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

#include "info_string_node.h"
#include "info_string.h"

#include <QGraphicsSimpleTextItem>
#include <QTableWidget>

namespace spaghetti::nodes {

InfoString::InfoString()
{
  QFont font{};
  font.setFamily("Consolas");
  font.setPointSize(10);
  auto widget = new QGraphicsSimpleTextItem("0");
  widget->setFont(font);

  auto brush = widget->brush();
  brush.setColor(Qt::white);
  widget->setBrush(brush);

  setCentralWidget(widget);

  m_info = widget;
}

void InfoString::refreshCentralWidget()
{
  if (!m_element) return;
  auto const value{ m_element->inputs()[0].getValue<std::string>() };
  m_info->setText(QString::fromStdString(value));

  calculateBoundingRect();
}

void InfoString::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

} // namespace spaghetti::nodes
