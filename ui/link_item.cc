#include "ui/link_item.h"

#include <cmath>

#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QStyleOptionGraphicsItem>

#include "config.h"
#include "ui/socket_item.h"

LinkItem::LinkItem(QGraphicsItem *a_parent)
  : QGraphicsPathItem{ a_parent }
  , m_effect{ new QGraphicsDropShadowEffect }
{
  m_effect->setBlurRadius(15);
  m_effect->setColor(QColor("#99050505"));
  setGraphicsEffect(m_effect);

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

void LinkItem::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *a_option, QWidget *a_widget)
{
  (void)a_option;
  (void)a_widget;

  Config const &config{ Config::get() };
  //  aPainter->setClipRect(aOption->exposedRect);
  QColor const signalColor{ m_isSignalOn ? m_colorSignalOn : m_colorSignalOff };
  QColor const notActive{ isSelected() ? config.getColor(Config::Color::eSelected) : signalColor };
  //                                       : config.getColor(Config::Color::eLink) };
  QColor const hover{ config.getColor(Config::Color::eSocketHover) };
  QPen pen{ (m_isHover ? hover : notActive) };
  pen.setStyle(m_to ? Qt::SolidLine : Qt::DashDotLine);
  pen.setWidth(3);

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

void LinkItem::setFrom(SocketItem *const a_from)
{
  m_from = a_from;

  QPointF const position{ mapFromScene(m_from->scenePos()) + QPointF(0, static_cast<qreal>(SocketItem::SIZE) / 2.) };
  setPos(position);

  trackNodes();
}

void LinkItem::setTo(SocketItem *const a_to)
{
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
  update();
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

  update();
}

void LinkItem::trackNodes()
{
  prepareGeometryChange();

  QPointF linkItemPos{ m_from->scenePos() };
  linkItemPos.ry() += m_from->boundingRect().height() / 2;
  setPos(linkItemPos);

  QPointF const toPoint{ (
      m_to ? (mapFromScene(m_to->scenePos()) + QPointF(0, static_cast<qreal>(SocketItem::SIZE) / 2.)) : m_toPoint) };
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

  double dist{ fabs(m_toPoint.x()) * 0.3 };

  c1.setX(dist);

  c2.setX(m_toPoint.x() - dist);
  c2.setY(m_toPoint.y());

  m_path = QPainterPath{};
  m_path.cubicTo(c1, c2, m_toPoint);

  QPainterPathStroker stroker{};
  stroker.setWidth(15);
  m_shape = stroker.createStroke(m_path);

  update();
}