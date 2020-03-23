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

#include <spaghettiui/node.h>

#include <bitset>
#include <cmath>
#include <iostream>

#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QTreeWidget>

#include <spaghetti/logger.h>
#include <spaghetti/package.h>
#include <spaghettiui/colors.h>
#include <spaghettiui/package_view.h>

namespace spaghetti {

constexpr int32_t const SOCKET_SIZE = SocketItem::SIZE;
qreal const ROUNDED_SOCKET_SIZE = std::round(static_cast<qreal>(SOCKET_SIZE) / 10.0) * 10.0;
qreal const ROUNDED_SOCKET_SIZE_2 = ROUNDED_SOCKET_SIZE / 2.0;

Node::Node(QGraphicsItem *const a_parent)
  : QGraphicsItem{ a_parent }
{
  m_nameFont.setFamily("Consolas");
  m_nameFont.setPointSize(8);

  setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

  iconify();
}

Node::~Node()
{
  for (auto &input : m_inputs) input->disconnectAll();
  for (auto &output : m_outputs) output->disconnectAll();

  if (m_element) {
    auto const package = m_element->package();
    package->remove(m_element);
  }
}

QRectF Node::boundingRect() const
{
  return m_boundingRect;
}

void Node::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  paintBorder(a_painter);
  if (!m_centralWidget || !m_centralWidget->isVisible()) paintIcon(a_painter);
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange a_change, QVariant const &a_value)
{
  (void)a_change;
  (void)a_value;

  switch (a_change) {
    case ItemSelectedHasChanged: {
      Node *lastSelected{};
      auto selectedItems = scene()->selectedItems();
      for (auto &&item : selectedItems) {
        if (item->type() == NODE_TYPE) lastSelected = static_cast<Node *>(item);
      }
      m_packageView->setSelectedNode(lastSelected);
      m_packageView->showProperties();
      break;
    }
    case ItemPositionChange: {
      QPointF const POSITION{ a_value.toPointF() };
      qreal const X{ std::round(POSITION.x() / 10.0) * 10.0 };
      qreal const Y{ std::round(POSITION.y() / 10.0) * 10.0 };
      return QPointF{ X, Y };
    }
    case ItemPositionHasChanged: {
      if (m_element) {
        QPointF const POSITION{ a_value.toPointF() };
        switch (m_type) {
          case Type::eElement: m_element->setPosition(POSITION.x(), POSITION.y()); break;
          case Type::eInputs: {
            auto const package = reinterpret_cast<Package *>(m_element);
            package->setInputsPosition(POSITION.x(), POSITION.y());
            break;
          }
          case Type::eOutputs: {
            auto const package = reinterpret_cast<Package *>(m_element);
            package->setOutputsPosition(POSITION.x(), POSITION.y());
            break;
          }
        }
      }
      break;
    }
    default: break;
  }

  return QGraphicsItem::itemChange(a_change, a_value);
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *a_event)
{
  auto const MODIFIERS = QApplication::keyboardModifiers();

  if (!((MODIFIERS & Qt::ControlModifier) && open())) (m_mode == Mode::eIconified) ? expand() : iconify();

  QGraphicsItem::mouseDoubleClickEvent(a_event);
}

void Node::advance(int a_phase)
{
  if (!a_phase) return;

  updateOutputs();

  refreshCentralWidget();

  update();
}

void Node::setElement(Element *const a_element)
{
  if (m_element) return;

  m_element = a_element;

  if (m_type == Type::eElement) m_element->registerEventHandler([this](Event const &a_event) { handleEvent(a_event); });

  auto const &INPUTS = m_element->inputs();
  auto const &OUTPUTS = m_element->outputs();

  switch (m_type) {
    case Type::eElement:
      for (size_t i = 0; i < INPUTS.size(); ++i) {
        QString const NAME{ QString::fromStdString(INPUTS[i].name) };
        addSocket(SocketType::eInput, static_cast<uint64_t>(i), NAME, INPUTS[i].type);
      }
      for (size_t i = 0; i < OUTPUTS.size(); ++i) {
        QString const NAME{ QString::fromStdString(OUTPUTS[i].name) };
        addSocket(SocketType::eOutput, static_cast<uint64_t>(i), NAME, OUTPUTS[i].type);
      }

      m_element->setPosition(x(), y());
      m_element->iconify(m_mode == Mode::eIconified);

      setName(QString::fromStdString(m_element->name()));
      updateOutputs();
      break;
    case Type::eInputs:
      for (size_t i = 0; i < INPUTS.size(); ++i) {
        QString const NAME{ QString::fromStdString(INPUTS[i].name) };
        addSocket(SocketType::eOutput, static_cast<uint64_t>(i), NAME, INPUTS[i].type);
      }
      break;
    case Type::eOutputs:
      for (size_t i = 0; i < OUTPUTS.size(); ++i) {
        QString const NAME{ QString::fromStdString(OUTPUTS[i].name) };
        addSocket(SocketType::eInput, static_cast<uint64_t>(i), NAME, OUTPUTS[i].type);
      }
      break;
  }

  elementSet();

  calculateBoundingRect();
}

void Node::setName(QString const &a_name)
{
  m_name = a_name;

  if (m_packageView) m_packageView->updateName(this);

  if (m_element) {
    m_element->setName(a_name.toStdString());
    setToolTip(QString("%1 (%2)").arg(a_name).arg(m_element->id()));
  } else
    setToolTip(QString("%1").arg(a_name));
}

void Node::setIcon(QString const &a_icon)
{
  m_iconPath = a_icon;
  m_icon.load(a_icon);
}

void Node::showName()
{
  m_showName = true;
  calculateBoundingRect();
}

void Node::hideName()
{
  m_showName = false;
  calculateBoundingRect();
}

void Node::iconify()
{
  if (m_element) {
    m_element->iconify(true);
    if (m_element->iconifyingHidesCentralWidget() && m_centralWidget) m_centralWidget->hide();
  }

  m_mode = Mode::eIconified;

  for (auto &&input : m_inputs) input->hideName();
  for (auto &&output : m_outputs) output->hideName();

  calculateBoundingRect();
}

void Node::expand()
{
  if (m_element) {
    m_element->iconify(false);
    if (m_element->iconifyingHidesCentralWidget() && m_centralWidget) m_centralWidget->show();
  }

  m_mode = Mode::eExpanded;

  for (auto &&input : m_inputs) input->showName();
  for (auto &&output : m_outputs) output->showName();

  calculateBoundingRect();
}

void Node::setPropertiesTable(QTableWidget *const a_properties)
{
  m_properties = a_properties;
}

void Node::paintBorder(QPainter *const a_painter)
{
  auto rect = boundingRect();

  QPen pen{};

  pen.setColor(get_color(Color::eSocketBorder));
  pen.setWidth(2);
  QColor color{ 105, 105, 105, 128 };
  QBrush brush{ color };
  a_painter->setPen(Qt::NoPen);
  a_painter->setBrush(brush);
  a_painter->drawRect(rect);

  if (m_showName) {
    QRectF nameRect{ 0.0, 0.0, m_boundingRect.width(), ROUNDED_SOCKET_SIZE };
    pen.setColor(get_color(Color::eFontName));
    QColor nameBackground{ get_color(Color::eNameBackground) };
    nameBackground.setAlpha(128);

    a_painter->setPen(Qt::NoPen);
    a_painter->setBrush(nameBackground);
    a_painter->drawRect(nameRect);

    pen.setColor(get_color(Color::eFontName));
    a_painter->setFont(m_nameFont);
    a_painter->setPen(pen);

    QFontMetrics const METRICS{ m_nameFont };
    int const FONT_HEIGHT = METRICS.height();
    qreal const NAME_Y = (ROUNDED_SOCKET_SIZE / 2.0) + (FONT_HEIGHT - METRICS.strikeOutPos()) / 2.0 - 1.0;

    a_painter->drawText(QPointF{ 5.0, NAME_Y }, m_name);
  }

  pen.setColor(isSelected() ? QColor(156, 156, 156, 255) : QColor(58, 66, 71, 255));
  pen.setWidth(2);
  a_painter->setPen(pen);
  a_painter->setBrush(Qt::NoBrush);
  a_painter->drawRect(rect);
}

void Node::paintIcon(QPainter *const a_painter)
{
  auto const HALF_ICON_SIZE = m_icon.size() / 2;

  auto const Y = static_cast<int>(m_centralWidgetPosition.y());
  auto const WIDTH = HALF_ICON_SIZE.width();
  auto const HEIGHT = HALF_ICON_SIZE.height();
  a_painter->drawPixmap(static_cast<int>(m_centralWidgetPosition.x()), Y, WIDTH, HEIGHT, m_icon);
}

void Node::showProperties()
{
  showCommonProperties();
  switch (m_type) {
    case Type::eElement:
      showIOProperties(IOSocketsType::eInputs);
      showIOProperties(IOSocketsType::eOutputs);
      break;
    case Type::eInputs: showIOProperties(IOSocketsType::eInputs); break;
    case Type::eOutputs: showIOProperties(IOSocketsType::eOutputs); break;
  }
}

void Node::handleEvent(Event const &a_event)
{
  switch (a_event.type) {
    case EventType::eElementNameChanged: break;
    case EventType::eIONameChanged: {
      auto const &EVENT = std::get<EventIONameChanged>(a_event.payload);
      changeIOName(EVENT.input ? IOSocketsType::eInputs : IOSocketsType::eOutputs, EVENT.id,
                   QString::fromStdString(EVENT.to));
      calculateBoundingRect();
      break;
    }
    case EventType::eIOTypeChanged: break;
    case EventType::eInputAdded: {
      auto const &INPUTS = m_element->inputs();
      auto const SIZE = inputs().size();
      auto const &INPUT = INPUTS.back();
      addSocket(SocketType::eInput, static_cast<uint64_t>(SIZE), QString::fromStdString(INPUT.name), INPUT.type);
      calculateBoundingRect();
      break;
    }
    case EventType::eInputRemoved:
      removeSocket(SocketType::eInput);
      calculateBoundingRect();
      break;
    case EventType::eOutputAdded: {
      auto const &OUTPUTS = m_element->outputs();
      auto const SIZE = outputs().size();
      auto const &OUTPUT = OUTPUTS.back();
      addSocket(SocketType::eOutput, static_cast<uint64_t>(SIZE), QString::fromStdString(OUTPUT.name), OUTPUT.type);
      calculateBoundingRect();
      break;
    }
    case EventType::eOutputRemoved:
      removeSocket(SocketType::eOutput);
      calculateBoundingRect();
      break;
  }
}

void Node::showCommonProperties()
{
  m_properties->setRowCount(0);
  propertiesInsertTitle("Element");

  QTableWidgetItem *item{};
  int const ID{ static_cast<int>(m_element->id()) };
  QString const TYPE{ QString::fromLocal8Bit(m_element->type()) };
  QString const NAME{ QString::fromStdString(m_element->name()) };

  int row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "ID" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  item = new QTableWidgetItem{ ID };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setData(Qt::DisplayRole, ID);
  m_properties->setItem(row, 1, item);

  row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Type" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  item = new QTableWidgetItem{ TYPE };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 1, item);

  row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Name" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  QLineEdit *nameEdit = new QLineEdit{ NAME };
  m_properties->setCellWidget(row, 1, nameEdit);
  QObject::connect(nameEdit, &QLineEdit::textChanged, [this](QString const &a_text) { setName(a_text); });
}

void Node::showIOProperties(IOSocketsType const a_type, bool const a_enabled)
{
  bool const INPUTS{ a_type == IOSocketsType::eInputs };
  auto &ios = INPUTS ? m_element->inputs() : m_element->outputs();

  int const IOS_SIZE{ static_cast<int>(ios.size()) };
  uint64_t const MIN_IOS_SIZE{ INPUTS ? m_element->minInputs() : m_element->minOutputs() };
  uint64_t const MAX_IOS_SIZE{ INPUTS ? m_element->maxInputs() : m_element->maxOutputs() };
  bool const ADDING_DISABLED{ MIN_IOS_SIZE == MAX_IOS_SIZE };

  QTableWidgetItem *item{};
  int row = m_properties->rowCount();

  propertiesInsertTitle(INPUTS ? "Inputs" : "Outputs");

  row = m_properties->rowCount();
  m_properties->insertRow(row);
  item = new QTableWidgetItem{ "Count" };
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  m_properties->setItem(row, 0, item);

  auto const count = new QSpinBox;
  count->setRange(MIN_IOS_SIZE, MAX_IOS_SIZE);
  count->setValue(static_cast<int>(ios.size()));
  m_properties->setCellWidget(row, 1, count);
  QObject::connect(count, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [INPUTS, this](int a_value) {
    auto const SIZE = INPUTS ? m_element->inputs().size() : m_element->outputs().size();
    if (static_cast<int>(SIZE) < a_value)
      INPUTS ? addInput() : addOutput();
    else
      INPUTS ? removeInput() : removeOutput();
  });
  count->setDisabled(ADDING_DISABLED);
  count->setEnabled(a_enabled);

  for (int i = 0; i < IOS_SIZE; ++i) {
    row = m_properties->rowCount();
    m_properties->insertRow(row);

    auto const &IO = ios[static_cast<size_t>(i)];

    if (IO.flags & Element::IOSocket::eCanChangeName) {
      QLineEdit *const ioName{ new QLineEdit{ QString::fromStdString(IO.name) } };
      QObject::connect(ioName, &QLineEdit::editingFinished, [a_type, i, ioName, this]() {
        m_element->setIOName(a_type == IOSocketsType::eInputs, static_cast<uint64_t>(i), ioName->text().toStdString());
      });
      m_properties->setCellWidget(row, 0, ioName);
    } else {
      item = new QTableWidgetItem{ QString::fromStdString(IO.name) };
      item->setFlags(item->flags() & ~Qt::ItemIsEditable);
      m_properties->setItem(row, 0, item);
    }

    auto const comboBox = new QComboBox;
    auto const holdedValues = ValueDescription::heldedValues(static_cast<IOSocketFlags::Flags>(IO.flags));

    for (auto const &value : holdedValues)
      comboBox->addItem(QString::fromStdString(ValueDescription::typeStdString(value)), static_cast<int>(value));

    int const INDEX{ comboBox->findData(static_cast<int>(IO.type)) };
    comboBox->setCurrentIndex(INDEX);
    m_properties->setCellWidget(row, 1, comboBox);
    QObject::connect(comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
                     [a_type, i, comboBox, this](int a_index) {
                       ValueType const VALUE_TYPE{ static_cast<ValueType>(comboBox->itemData(a_index).toInt()) };
                       setSocketType(a_type, static_cast<uint64_t>(i), VALUE_TYPE);
                     });
    comboBox->setEnabled(a_enabled);
  }
}

void Node::setCentralWidget(QGraphicsItem *a_centralWidget)
{
  if (m_centralWidget) delete m_centralWidget;
  m_centralWidget = a_centralWidget;
  m_centralWidget->setParentItem(this);
  m_centralWidget->setPos(m_centralWidgetPosition);
}

void Node::propertiesInsertTitle(QString const &a_title)
{
  int const ROW = m_properties->rowCount();
  m_properties->insertRow(ROW);
  QTableWidgetItem *const item{ new QTableWidgetItem{ a_title } };
  item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
  item->setBackground(QBrush(Qt::darkGray));
  item->setForeground(QBrush(Qt::black));

  m_properties->setItem(ROW, 0, item);
  m_properties->setSpan(ROW, 0, 1, 2);
}

void Node::changeIOName(IOSocketsType const a_type, int const a_id, QString const &a_name)
{
  bool const INPUTS{ a_type == IOSocketsType::eInputs };

  SocketItem *socket{};
  if (m_type == Type::eElement)
    socket = INPUTS ? m_inputs[a_id] : m_outputs[a_id];
  else
    socket = m_type == Type::eInputs ? m_outputs[a_id] : m_inputs[a_id];

  socket->setName(a_name);
  calculateBoundingRect();
}

template<typename Container, class Comparator>
auto max_element(Container &a_container, Comparator a_comparator)
{
  return std::max_element(std::begin(a_container), std::end(a_container), a_comparator);
}

void Node::calculateBoundingRect()
{
  prepareGeometryChange();

  auto const INPUTS_COUNT = m_inputs.count();
  auto const OUTPUTS_COUNT = m_outputs.count();
  auto const SOCKETS_COUNT = std::max(INPUTS_COUNT, OUTPUTS_COUNT);
  auto const CENTRAL_SIZE =
      (m_centralWidget && m_centralWidget->isVisible()) ? m_centralWidget->boundingRect().size() : m_icon.size() / 2;
  auto const SOCKETS_HEIGHT = SOCKETS_COUNT * ROUNDED_SOCKET_SIZE;

  auto maxNameWidth = [](auto &&a_a, auto &&a_b) { return a_a->nameWidth() < a_b->nameWidth(); };
  auto const LONGEST_INPUT = max_element(m_inputs, maxNameWidth);
  auto const LONGEST_OUTPUT = max_element(m_outputs, maxNameWidth);
  int const LONGEST_INPUTS_NAME_WIDTH = LONGEST_INPUT != std::end(m_inputs) ? (*LONGEST_INPUT)->nameWidth() : 0;
  int const LONGEST_OUTPUTS_NAME_WIDTH = LONGEST_OUTPUT != std::end(m_outputs) ? (*LONGEST_OUTPUT)->nameWidth() : 0;
  int const INPUTS_NAME_WIDTH = m_mode == Mode::eExpanded ? LONGEST_INPUTS_NAME_WIDTH : 0;
  int const OUTPUTS_NAME_WIDTH = m_mode == Mode::eExpanded ? LONGEST_OUTPUTS_NAME_WIDTH : 0;
  int const NAME_OFFSET = m_showName ? static_cast<int>(ROUNDED_SOCKET_SIZE_2) : 0;

  qreal width{ CENTRAL_SIZE.width() };
  qreal height{};

  if (SOCKETS_HEIGHT > CENTRAL_SIZE.height())
    height = NAME_OFFSET + SOCKETS_HEIGHT + ROUNDED_SOCKET_SIZE;
  else {
    height = NAME_OFFSET + CENTRAL_SIZE.height() + ROUNDED_SOCKET_SIZE_2;
    if (SOCKETS_COUNT < 2) height += ROUNDED_SOCKET_SIZE_2;
  }

  width = ROUNDED_SOCKET_SIZE + INPUTS_NAME_WIDTH + CENTRAL_SIZE.width() + OUTPUTS_NAME_WIDTH + ROUNDED_SOCKET_SIZE;
  width = std::round(width / 10.0) * 10.0;
  height = std::round(height / 10.0) * 10.0;

  qreal const CENTRAL_X = ROUNDED_SOCKET_SIZE + INPUTS_NAME_WIDTH;
  qreal const CENTRAL_Y = NAME_OFFSET + (height / 2.0) - (CENTRAL_SIZE.height() / 2.0);
  m_centralWidgetPosition = QPointF{ CENTRAL_X, CENTRAL_Y };
  if (m_centralWidget) m_centralWidget->setPos(m_centralWidgetPosition);

  qreal yOffset{ ROUNDED_SOCKET_SIZE + NAME_OFFSET };
  for (auto &&input : m_inputs) {
    input->setPos(0.0, yOffset);
    yOffset += ROUNDED_SOCKET_SIZE;
  }

  yOffset = ROUNDED_SOCKET_SIZE + NAME_OFFSET;
  for (auto &&output : m_outputs) {
    output->setPos(width, yOffset);
    yOffset += ROUNDED_SOCKET_SIZE;
  }

  m_boundingRect = QRectF{ 0.0, 0.0, width, height };
}

void Node::changeInputName(int const a_id, QString const &a_name)
{
  changeIOName(IOSocketsType::eInputs, a_id, a_name);
}

void Node::changeOutputName(int const a_id, QString const &a_name)
{
  changeIOName(IOSocketsType::eOutputs, a_id, a_name);
}

QString nodetype2string(Node::Type a_type)
{
  switch (a_type) {
    case Node::Type::eElement: return "Element";
    case Node::Type::eInputs: return "Inputs";
    case Node::Type::eOutputs: return "Outputs";
  }

  return "UNKNOWN TYPE";
}

void Node::addInput()
{
  uint64_t const SIZE{ static_cast<uint64_t>(m_element->inputs().size()) };
  QString const INPUT_NAME{ QString("#%1").arg(SIZE + 1) };

  ValueType const TYPE{ ValueDescription::firstAvailableTypeForFlags(
      static_cast<IOSocketFlags::Flags>(m_element->defaultNewInputFlags())) };
  m_element->addInput(TYPE, INPUT_NAME.toStdString(), m_element->defaultNewInputFlags());

  m_packageView->showProperties();
}

void Node::removeInput()
{
  switch (m_type) {
    case Type::eInputs:
      if (m_outputs.size()) m_outputs.last()->disconnectAll();
      break;
    case Type::eOutputs:
      if (m_inputs.size()) m_inputs.last()->disconnectAll();
      break;
    case Type::eElement:
      if (m_inputs.size()) m_inputs.last()->disconnectAll();
      if (m_dedicatedInput && m_dedicatedInput->outputs().size()) m_dedicatedInput->outputs().last()->disconnectAll();
      break;
  }

  m_element->removeInput();
  m_packageView->showProperties();
}

void Node::setInputName(uint64_t const a_socketId, QString const &a_name)
{
  m_element->setInputName(a_socketId, a_name.toStdString());
  m_inputs[a_socketId]->setName(a_name);
  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::addOutput()
{
  uint64_t const SIZE{ static_cast<uint64_t>(m_element->outputs().size()) };
  QString const OUTPUT_NAME{ QString("#%1").arg(SIZE + 1) };

  ValueType const TYPE{ ValueDescription::firstAvailableTypeForFlags(
      static_cast<IOSocketFlags::Flags>(m_element->defaultNewInputFlags())) };
  m_element->addOutput(TYPE, OUTPUT_NAME.toStdString(), m_element->defaultNewOutputFlags());

  m_packageView->showProperties();
}

void Node::removeOutput()
{
  switch (m_type) {
    case Type::eInputs:
      if (m_outputs.size()) m_outputs.last()->disconnectAll();
      break;
    case Type::eOutputs:
      if (m_inputs.size()) m_inputs.last()->disconnectAll();
      break;
    case Type::eElement:
      if (m_outputs.size()) m_outputs.last()->disconnectAll();
      if (m_dedicatedOutput && m_dedicatedOutput->inputs().size()) m_dedicatedOutput->inputs().last()->disconnectAll();
      break;
  }

  m_element->removeOutput();
  m_packageView->showProperties();
}

void Node::setOutputName(uint64_t const a_socketId, QString const &a_name)
{
  m_element->setOutputName(a_socketId, a_name.toStdString());
  m_outputs[a_socketId]->setName(a_name);
  calculateBoundingRect();
  m_packageView->showProperties();
}

void Node::addSocket(SocketType const a_type, uint64_t const a_id, QString const &a_name, ValueType const a_valueType)
{
  auto const socket = new SocketItem{ this, a_type };
  socket->setElementId(m_type == Type::eElement ? m_element->id() : 0);
  socket->setSocketId(a_id);

  socket->setName(a_name);
  socket->setValueType(a_valueType);

  if (m_mode == Mode::eIconified)
    socket->hideName();
  else
    socket->showName();

  if (a_type == SocketType::eInput)
    m_inputs.push_back(socket);
  else
    m_outputs.push_back(socket);
}

void Node::removeSocket(Node::SocketType const a_type)
{
  switch (a_type) {
    case SocketType::eInput:
      delete m_inputs.back();
      m_inputs.pop_back();
      break;
    case SocketType::eOutput:
      delete m_outputs.back();
      m_outputs.pop_back();
      break;
  }
}

void Node::setSocketType(IOSocketsType const a_socketType, uint64_t const a_socketId, ValueType const a_type)
{
  assert(m_element);

  bool const INPUTS{ a_socketType == IOSocketsType::eInputs };
  auto &io = INPUTS ? m_element->inputs()[a_socketId] : m_element->outputs()[a_socketId];

  if (!ValueDescription::isTypeAlowed(a_type, static_cast<IOSocketFlags::Flags>(io.flags))) {
    spaghetti::log::error("Changing io's {}@{} type to {} is not allowed.", m_element->id(), io.id,
                          ValueDescription::typeStdString(a_type));
    return;
  }

  SocketItem *socket{};
  if (m_type == Type::eElement)
    socket = INPUTS ? m_inputs[a_socketId] : m_outputs[a_socketId];
  else
    socket = m_type == Type::eInputs ? m_outputs[a_socketId] : m_inputs[a_socketId];

  if (socket->valueType() == a_type) return;

  socket->disconnectAll();

  m_element->setIOValueType(INPUTS, a_socketId, a_type);

  socket->setValueType(a_type);
}

void Node::updateOutputs()
{
  if (!m_element || m_type == Type::eOutputs) return;

  auto const IS_ELEMENT = m_type == Type::eElement;
  auto const &ELEMENT_IOS = IS_ELEMENT ? m_element->outputs() : m_element->inputs();
  auto const &NODE_IOS = m_outputs;
  size_t const SIZE{ ELEMENT_IOS.size() };
  for (size_t i = 0; i < SIZE; ++i) {
    switch (ELEMENT_IOS[i].type) {
      case ValueType::eBool: {
        bool const SIGNAL{ ELEMENT_IOS[i].getValueWithoutNotify<bool>() };
        NODE_IOS[static_cast<int>(i)]->setSignal(SIGNAL);
        break;
      }
      default: break;
    }
  }
}

} // namespace spaghetti
