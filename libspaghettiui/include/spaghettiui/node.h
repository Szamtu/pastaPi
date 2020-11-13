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

#pragma once
#ifndef SPAGHETTI_NODE_H
#define SPAGHETTI_NODE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QVector>

#include <spaghetti/element.h>
#include <spaghettiui/api_ui.h>
#include <spaghettiui/socket_item.h>

#include <QString>

class QTableWidget;

namespace spaghetti {

constexpr int NODE_TYPE{ QGraphicsItem::UserType + 1 };

class PackageView;

class SPAGHETTI_API_UI Node : public QGraphicsItem {
 public:
  using Sockets = QVector<SocketItem *>;

  explicit Node(QGraphicsItem *const a_parent = nullptr);
  ~Node() override;

  int type() const override { return NODE_TYPE; }
  QRectF boundingRect() const override;
  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override;
  QVariant itemChange(GraphicsItemChange a_change, QVariant const &a_value) override;
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *a_event) override;
  void advance(int a_phase) override;

  enum class Type { eElement, eInputs, eOutputs };
  enum class IOSocketsType { eInputs, eOutputs };
  using SocketType = SocketItem::Type;

  void setType(Type const a_type) { m_type = a_type; }

  PackageView *packageView() const { return m_packageView; }
  void setPackageView(PackageView *const a_packageView) { m_packageView = a_packageView; }

  void setElement(Element *const a_element);

  void setName(QString const &a_name);
  QString name() const { return m_name; }

  void setPath(QString const &a_path) { m_path = a_path; }
  QString path() const { return m_path; }

  void setIcon(QString const &a_icon);
  QPixmap icon() const { return m_icon; }
  QString iconPath() const { return m_iconPath; }

  void setLocked(bool a_isLocked) { m_isLocked = a_isLocked; }
  bool isLocked() const { return m_isLocked; }

  void showName();
  void hideName();

  void iconify();
  void expand();

  Element *element() const { return m_element; }

  Sockets const &inputs() const { return m_inputs; }
  Sockets const &outputs() const { return m_outputs; }

  void setDedicatedInput(Node *a_node) { m_dedicatedInput = a_node; }
  void setDedicatedOutput(Node *a_node) { m_dedicatedOutput = a_node; }

  void setPropertiesTable(QTableWidget *const a_properties);

  void paintBorder(QPainter *const a_painter);
  void paintIcon(QPainter *const a_painter);

  virtual void showProperties();
  virtual void refreshCentralWidget() {}
  virtual void elementSet() {}
  virtual void handleEvent(Event const &a_event);
  virtual bool open() { return false; }

  void showCommonProperties();
  void showIOProperties(IOSocketsType const a_type, bool const a_enabled = true);

  void calculateBoundingRect();

  void changeInputName(int const a_id, QString const &a_name);
  void changeOutputName(int const a_id, QString const &a_name);
  void addSocket(SocketType const a_type, uint64_t const a_id, QString const &a_name, ValueType const a_valueType);
  void removeSocket(SocketType const a_type);
  void setSocketType(IOSocketsType const a_socketType, uint64_t const a_socketId, ValueType const a_type);

 public:
  void addPropertyString(QString const a_propertyName, std::string *a_destMember);
  void addPropertyInt(QString const a_propertyName, int *a_destMember);
  void addPropertyBool(QString const a_propertyName, bool *a_destMember);
  void addPropertyDouble(QString const a_propertyName, double *a_destMember);

 private:
  int addPropertyRow(QString const a_propertyName);

 protected:
  void setCentralWidget(QGraphicsItem *a_centralWidget);
  void propertiesInsertTitle(QString const &a_title);
  void changeIOName(IOSocketsType const a_type, int const a_id, QString const &a_name);

 private:
  void addInput();
  void removeInput();
  void setInputName(uint64_t const a_socketId, QString const &a_name);

  void addOutput();
  void removeOutput();
  void setOutputName(uint64_t const a_socketId, QString const &a_name);

  void updateOutputs();

 protected:
  QGraphicsItem *m_centralWidget{};
  QTableWidget *m_properties{};
  Element *m_element{};
  PackageView *m_packageView{};

 private:
  enum class Mode { eIconified, eExpanded } m_mode{};
  Type m_type{};
  bool m_showName{ true };
  QString m_name{};
  QString m_path{};
  QString m_iconPath{};
  QPixmap m_icon{};
  bool m_isLocked{ false };
  QRectF m_boundingRect{};
  QPointF m_centralWidgetPosition{};

  Sockets m_inputs{};
  Sockets m_outputs{};

  Node *m_dedicatedInput{};
  Node *m_dedicatedOutput{};

  QFont m_nameFont{};
};

} // namespace spaghetti

#endif // SPAGHETTI_NODE_H
