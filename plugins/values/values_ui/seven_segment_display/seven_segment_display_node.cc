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

#include "seven_segment_display_node.h"
#include "seven_segment_display.h"

#include <QTableWidget>

namespace spaghetti::nodes {

class SevenSegmentDisplayWidget : public QGraphicsItem {
 public:
  SevenSegmentDisplayWidget()
    : QGraphicsItem{}
  {
    createSegments();
  }

  QRectF boundingRect() const override { return m_boundingRect; }

  void paint(QPainter *a_painter, QStyleOptionGraphicsItem const *a_option, QWidget *a_widget) override
  {
    (void)a_option;
    (void)a_widget;

    QBrush const ON{ Qt::black };
    QBrush const OFF{ QColor(90, 90, 90, 255) };

    a_painter->translate(10.0, 10.0);
    a_painter->setPen(Qt::NoPen);

    a_painter->setBrush(m_states[0] ? ON : OFF);
    a_painter->drawPolygon(m_segments[0]);

    a_painter->setBrush(m_states[1] ? ON : OFF);
    a_painter->drawPolygon(m_segments[1]);

    a_painter->setBrush(m_states[2] ? ON : OFF);
    a_painter->drawPolygon(m_segments[2]);

    a_painter->setBrush(m_states[3] ? ON : OFF);
    a_painter->drawPolygon(m_segments[3]);

    a_painter->setBrush(m_states[4] ? ON : OFF);
    a_painter->drawPolygon(m_segments[4]);

    a_painter->setBrush(m_states[5] ? ON : OFF);
    a_painter->drawPolygon(m_segments[5]);

    a_painter->setBrush(m_states[6] ? ON : OFF);
    a_painter->drawPolygon(m_segments[6]);

    a_painter->setBrush(m_states[7] ? ON : OFF);
    a_painter->drawEllipse(m_segments[7].boundingRect());
  }

  void setState(size_t const a_index, bool const a_value) { m_states[a_index] = a_value; }

 private:
  void createSegments()
  {
    // Segment A
    m_segments[0] << QPointF{ 22.0, 19.0 } << QPointF{ 38.0, 4.0 } << QPointF{ 96.0, 4.0 } << QPointF{ 111.0, 19.0 }
                  << QPointF{ 96.0, 34.0 } << QPointF{ 38.0, 34.0 };

    // Segment B
    m_segments[1] << QPointF{ 115.0, 23.0 } << QPointF{ 131.0, 37.0 } << QPointF{ 131.0, 83.0 }
                  << QPointF{ 115.0, 98.0 } << QPointF{ 99.0, 83.0 } << QPointF{ 99.0, 37.0 };

    // Segment C
    m_segments[2] << QPointF{ 115.0, 105.0 } << QPointF{ 131.0, 119.0 } << QPointF{ 131.0, 165.0 }
                  << QPointF{ 115.0, 180.0 } << QPointF{ 99.0, 165.0 } << QPointF{ 99.0, 119.0 };

    // Segment D
    m_segments[3] << QPointF{ 22.0, 183.0 } << QPointF{ 38.0, 167.0 } << QPointF{ 96.0, 167.0 }
                  << QPointF{ 111.0, 183.0 } << QPointF{ 96.0, 199.0 } << QPointF{ 38.0, 199.0 };

    // Segment E
    m_segments[4] << QPointF{ 19.0, 105.0 } << QPointF{ 35.0, 119.0 } << QPointF{ 35.0, 165.0 }
                  << QPointF{ 19.0, 180.0 } << QPointF{ 4.0, 165.0 } << QPointF{ 4.0, 119.0 };

    // Segment F
    m_segments[5] << QPointF{ 19.0, 23.0 } << QPointF{ 35.0, 37.0 } << QPointF{ 35.0, 83.0 } << QPointF{ 19.0, 98.0 }
                  << QPointF{ 4.0, 83.0 } << QPointF{ 4.0, 37.0 };

    // Segment G
    m_segments[6] << QPointF{ 22.0, 101.0 } << QPointF{ 38.0, 85.0 } << QPointF{ 96.0, 85.0 } << QPointF{ 111.0, 101.0 }
                  << QPointF{ 96.0, 116.0 } << QPointF{ 38.0, 116.0 };

    // Segment DP
    m_segments[7] << QPointF{ 133.0, 162.0 } << QPointF{ 171.0, 162.0 } << QPointF{ 171.0, 199.0 }
                  << QPointF{ 133.0, 199.0 };
  }

 private:
  QPolygonF m_segments[8]{};
  QRectF m_boundingRect{ 0, 0, 194, 222 };
  bool m_states[8]{};
};

SevenSegmentDisplay::SevenSegmentDisplay()
{
  auto const widget = new SevenSegmentDisplayWidget;
  setCentralWidget(widget);

  m_widget = widget;
}

void SevenSegmentDisplay::refreshCentralWidget()
{
  if (!m_element) return;

  auto &inputs = m_element->inputs();

  bool const A{ inputs[0].getValue<bool>() };
  bool const B{ inputs[1].getValue<bool>() };
  bool const C{ inputs[2].getValue<bool>() };
  bool const D{ inputs[3].getValue<bool>() };
  bool const E{ inputs[4].getValue<bool>() };
  bool const F{ inputs[5].getValue<bool>() };
  bool const G{ inputs[6].getValue<bool>() };
  bool const DP{ inputs[7].getValue<bool>() };

  m_widget->setState(0, A);
  m_widget->setState(1, B);
  m_widget->setState(2, C);
  m_widget->setState(3, D);
  m_widget->setState(4, E);
  m_widget->setState(5, F);
  m_widget->setState(6, G);
  m_widget->setState(7, DP);

  calculateBoundingRect();
}

void SevenSegmentDisplay::showProperties()
{
  showCommonProperties();
  showIOProperties(IOSocketsType::eInputs);
  showIOProperties(IOSocketsType::eOutputs);
}

} // namespace spaghetti::nodes
