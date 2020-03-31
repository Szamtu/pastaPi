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

#include "yolo_node.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QGraphicsSimpleTextItem>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>

namespace spaghetti::nodes {

Yolo::Yolo() {}

void Yolo::refreshCentralWidget() {}

void Yolo::showProperties()
{
  showCommonProperties();
  auto const CONST_YOLO = static_cast<elements::Yolo *>(m_element);
  m_configuration = CONST_YOLO->configuration();

  propertiesInsertTitle("Yolo Setup");

  int row = m_properties->rowCount();
  m_properties->insertRow(row);

  QTableWidgetItem *modelItem{};

  modelItem = new QTableWidgetItem{ "Model" };
  modelItem->setFlags(modelItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, modelItem);

  QPushButton *modelSelectButton = new QPushButton{ "Select" };
  m_properties->setCellWidget(row, 1, modelSelectButton);

  QObject::connect(modelSelectButton, &QPushButton::clicked, [this]() {
    auto const currentFile = QString::fromStdString(m_configuration.modelConfigurationPath);
    QString dir = QFileDialog::getOpenFileName(nullptr, "Select model configuration file", currentFile);
    m_configuration.modelConfigurationPath = dir.toStdString();
  });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *weightsItem{};
  weightsItem = new QTableWidgetItem{ "Weights" };
  weightsItem->setFlags(modelItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, weightsItem);

  QPushButton *wegihtsSelectButton = new QPushButton{ "Select" };
  m_properties->setCellWidget(row, 1, wegihtsSelectButton);

  QObject::connect(wegihtsSelectButton, &QPushButton::clicked, [this]() {
    auto const currentFile = QString::fromStdString(m_configuration.weightsFilePath);
    QString dir = QFileDialog::getOpenFileName(nullptr, "Select weights configuration file", currentFile);
    m_configuration.weightsFilePath = dir.toStdString();
  });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *cudaItem{};
  cudaItem = new QTableWidgetItem{ "CUDA" };
  cudaItem->setFlags(cudaItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, cudaItem);

  QCheckBox *const cudaCheckbox = new QCheckBox;
  m_properties->setCellWidget(row, 1, cudaCheckbox);
  cudaCheckbox->setChecked(m_configuration.cudaEnabled);
  QObject::connect(cudaCheckbox, &QCheckBox::stateChanged, [this, cudaCheckbox, CONST_YOLO](int a_state) {
    if (a_state == 2) {
      if (!CONST_YOLO->isCudaAvailable()) {
        QMessageBox::warning(nullptr, "Enabling CUDA failed!",
                             "Cannot run CUDA! Please check if opencv was built with CUDA support and you have "
                             "installed GPU with CUDA support.");
        cudaCheckbox->setChecked(false);
        m_configuration.cudaEnabled = false;
      } else {
        m_configuration.cudaEnabled = true;
      }
    } else {
      m_configuration.cudaEnabled = false;
    }
  });

  row++;
  m_properties->insertRow(row);
  QTableWidgetItem *applyItem{};
  applyItem = new QTableWidgetItem{ "Apply" };
  applyItem->setFlags(applyItem->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, applyItem);

  QPushButton *applyButton = new QPushButton{ "Apply" };
  m_properties->setCellWidget(row, 1, applyButton);

  QObject::connect(applyButton, &QPushButton::clicked, [this, CONST_YOLO]() {
    if (!CONST_YOLO->confugure(m_configuration)) {
      QMessageBox::warning(nullptr, "Configuration failed!", QString::fromStdString(CONST_YOLO->lastError()));
    }
  });

  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

} // namespace spaghetti::nodes
