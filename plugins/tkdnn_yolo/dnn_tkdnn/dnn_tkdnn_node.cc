// tkDNN YOLO PastaPi plugin
// Copyright (C) 2020  Pawel Adamski

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

// This plugin uses tkDNN
// Licensed under terms of GPLv2
// https://github.com/ceccocats/tkDNN

#include "dnn_tkdnn_node.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGraphicsSimpleTextItem>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes {

TkDnn::TkDnn() {}

void TkDnn::refreshCentralWidget() {}

void TkDnn::showProperties()
{
  showCommonProperties();
  auto const CONST_TKDNN = static_cast<elements::TkDnn *>(m_element);
  m_configuration = CONST_TKDNN->configuration();

  propertiesInsertTitle("tkDNN Setup");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);
  QTableWidgetItem *modelItem{};
  modelItem = new QTableWidgetItem{ "Model" };
  modelItem->setFlags(modelItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, modelItem);

  QPushButton *modelSelectButton = new QPushButton{ "Select" };
  m_properties->setCellWidget(row, 1, modelSelectButton);

  QObject::connect(modelSelectButton, &QPushButton::clicked, [this]() {
    auto const currentFile = QString::fromStdString(m_configuration.networkFilePath);
    QString dir = QFileDialog::getOpenFileName(nullptr, "Select model configuration file", currentFile);
    m_configuration.networkFilePath = dir.toStdString();
  });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *classesNumItem{};
  classesNumItem = new QTableWidgetItem{ "Classes" };
  classesNumItem->setFlags(modelItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, classesNumItem);

  QSpinBox *classesWidget = new QSpinBox{};
  classesWidget->setRange(1, 512);
  classesWidget->setValue(m_configuration.classes);
  m_properties->setCellWidget(row, 1, classesWidget);

  QObject::connect(classesWidget, QOverload<int>::of(&QSpinBox::valueChanged),
                   [this](int a_value) { m_configuration.classes = a_value; });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *confidenceThresholdItem{};
  confidenceThresholdItem = new QTableWidgetItem{ "Threshold" };
  confidenceThresholdItem->setFlags(modelItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, confidenceThresholdItem);

  QDoubleSpinBox *confidenceThresholdWidget = new QDoubleSpinBox{};
  confidenceThresholdWidget->setRange(0.01, 1.0);
  confidenceThresholdWidget->setValue(m_configuration.confidenceThreshold);
  confidenceThresholdWidget->setSingleStep(0.1);
  m_properties->setCellWidget(row, 1, confidenceThresholdWidget);

  QObject::connect(confidenceThresholdWidget, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                   [this](double a_value) { m_configuration.confidenceThreshold = a_value; });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *syncInputsItem{};
  syncInputsItem = new QTableWidgetItem{ "Sync inputs" };
  syncInputsItem->setFlags(syncInputsItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, syncInputsItem);

  QCheckBox *syncInputsWidget = new QCheckBox{};
  syncInputsWidget->setChecked(m_configuration.syncInputs);
  m_properties->setCellWidget(row, 1, syncInputsWidget);

  QObject::connect(syncInputsWidget, &QCheckBox::stateChanged,
                   [this](int a_value) { m_configuration.syncInputs = a_value; });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *applyItem{};
  applyItem = new QTableWidgetItem{ "Apply" };
  applyItem->setFlags(applyItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, applyItem);

  QPushButton *applyButton = new QPushButton{ "Apply" };
  m_properties->setCellWidget(row, 1, applyButton);

  QObject::connect(applyButton, &QPushButton::clicked, [this, CONST_TKDNN]() {
    if (!CONST_TKDNN->confugure(m_configuration)) {
      QMessageBox::warning(nullptr, "Configuration failed!", "Failed to configure tkDNN");
    }
  });

  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

} // namespace spaghetti::nodes
