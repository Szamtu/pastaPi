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

#include "sunrise_node.h"
#include "sunrise.h"

#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes {

Sunrise::Sunrise(){};

void Sunrise::showProperties()
{
  showCommonProperties();
  auto element = static_cast<elements::Sunrise *>(m_element);
  propertiesInsertTitle("Setup");
  int currentIndex = m_properties->rowCount();

  m_properties->insertRow(currentIndex);
  QTableWidgetItem *latItem = new QTableWidgetItem{"Lat"};
  latItem->setFlags(latItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, latItem);
  QLineEdit *latEdit = new QLineEdit{};
  m_properties->setCellWidget(currentIndex,1,latEdit);
  QObject::connect(latEdit, &QLineEdit::textChanged,
                   [this](const QString &a_text) { m_lat = a_text; });
  auto lat = element->lat();
  latEdit->setText(lat);

  currentIndex++;
  m_properties->insertRow(currentIndex);
  QTableWidgetItem *lonItem = new QTableWidgetItem{"Lon"};
  lonItem->setFlags(lonItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(currentIndex, 0, lonItem);
  QLineEdit *lonEdit = new QLineEdit{};
  m_properties->setCellWidget(currentIndex,1,lonEdit);
  QObject::connect(lonEdit, &QLineEdit::textChanged,
                   [this](const QString &a_text) { m_lon = a_text; });
  auto lon = element->lon();
  lonEdit->setText(lon);

  currentIndex++;
  m_properties->insertRow(currentIndex);
  QTableWidgetItem *itemApply = new QTableWidgetItem{"Apply"};
  itemApply->setFlags(itemApply->flags() & ~Qt::ItemIsEditable);
  QPushButton * applyButton = new QPushButton{"Apply"};
  m_properties->setCellWidget(currentIndex,1,applyButton);
  QObject::connect(applyButton, &QPushButton::clicked, [element, this]() {
    element->apply(m_lat,m_lon);
  });

 showIOProperties(IOSocketsType::eOutputs);

}

} // namespace spaghetti::nodes
