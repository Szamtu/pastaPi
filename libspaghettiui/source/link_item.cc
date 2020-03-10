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

#include <spaghettiui/colors.h>
#include <spaghettiui/link_item.h>
#include <spaghettiui/socket_item.h>
#include <QStyleOptionGraphicsItem>
#include <cmath>

namespace spaghetti {

LinkItem::LinkItem(QGraphicsItem *a_parent)
  : QGraphicsPathItem{ a_parent }
{
  setFlags(ItemSendsGeometryChanges | ItemIsFocusable | ItemIsSelectable);
  setZValue(-1);
  setAcceptHoverEvents(true);
}

QRectF LinkItem::boundingRect() const
{
  return m_shape.boundingRect();
}

QPainterPath LinkItem::shape() const
{
  return m_shape;
}

void LinkItem::paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  QColor const signalColor{ (m_isSignalOn ? m_colorSignalOn : m_colorSignalOff) };
  QColor const notActive{ (isSelected() ? get_color(Color::eSelected) : signalColor) };
  QColor const hover{ get_color(Color::eSocketHover) };

  QPen pen{ (m_isHover ? hover : notActive) };
  pen.setStyle((m_to ? Qt::SolidLine : Qt::DashDotLine));
  pen.setWidth(2);

  if (m_valueType != ValueType::eBool) {
    QPen dash = pen;
    QColor hover2 = signalColor;
    hover2.setAlpha(85);
    dash.setColor(hover2);
    dash.setStyle(Qt::DotLine);
    dash.setWidth(6);
    dash.setDashOffset(m_dashOffset);
    a_painter->setPen(dash);
    a_painter->drawPath(m_path);
  }

  a_painter->setPen(pen);
  a_painter->drawPath(m_path);
}

void LinkItem::hoverEnterEvent(QGraphicsSceneHoverEvent *a_event)
{
  (void)a_event;

  setHover(true);
}

void LinkItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *a_event)
{
  (void)a_event;
  setHover(false);
}

void LinkItem::advance(int a_phase)
{
  if (!a_phase) return;

  if (m_valueType != ValueType::eBool) m_dashOffset -= 0.1;

  update();
}

void LinkItem::setFrom(SocketItem *const a_from)
{
  m_from = a_from;

  QPointF const position{ mapFromScene(m_from->scenePos()) };
  setPos(position);

  trackNodes();
}

void LinkItem::setTo(SocketItem *const a_to)
{
  if (m_to) m_to->setSignal(false);

  m_to = a_to;
  m_isSnapped = a_to != nullptr;

  trackNodes();
}

void LinkItem::setTo(QPointF const a_to)
{
  m_toPoint = mapFromScene(a_to);

  trackNodes();
}

void LinkItem::setHover(bool const a_hover)
{
  m_isHover = a_hover;
  if (m_from) m_from->setHover(m_isHover);
  if (m_to) m_to->setHover(m_isHover);
}

void LinkItem::setColors(QColor const a_signalOff, QColor const a_signalOn)
{
  m_colorSignalOff = a_signalOff;
  m_colorSignalOn = a_signalOn;
}

void LinkItem::setSignal(bool const a_signal)
{
  m_isSignalOn = a_signal;

  if (m_to) m_to->setSignal(a_signal);
}

void LinkItem::trackNodes()
{
  prepareGeometryChange();

  QPointF const linkItemPos{ m_from->scenePos() };
  setPos(linkItemPos);

  QPointF const toPoint{ (m_to ? mapFromScene(m_to->scenePos()) : m_toPoint) };
  m_toPoint = toPoint;

  double x = toPoint.x() < 0. ? toPoint.x() : 0.;
  double y = toPoint.y() < 0. ? toPoint.y() : 0.;
  double w = fabs(toPoint.x());
  double h = fabs(toPoint.y());

  m_boundingRect.setX(x);
  m_boundingRect.setY(y);
  m_boundingRect.setWidth(w);
  m_boundingRect.setHeight(h);

  QPointF c1{}, c2{};

  double dist{ fabs(m_toPoint.x()) * 0.5 };

  c1.setX(dist);

  c2.setX(m_toPoint.x() - dist);
  c2.setY(m_toPoint.y());

  m_path = QPainterPath{};
  m_path.cubicTo(c1, c2, m_toPoint);

  QPainterPathStroker stroker{};
  stroker.setWidth(15);
  m_shape = stroker.createStroke(m_path);
}

} // namespace spaghetti
