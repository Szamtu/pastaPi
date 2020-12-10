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

#include "sunrise.h"

#include <QDebug>
#include <QDate>
#include <QTimeZone>
#include <qmath.h>

namespace spaghetti::elements {

Sunrise::Sunrise()
  : Element{},
    m_lat(),
    m_lon()
{
  setMinInputs(0);
  setMaxInputs(0);
  setMinOutputs(4);
  setMaxOutputs(4);

  addOutput(ValueType::eBool, "State", IOSocket::eCanHoldBool);
  addOutput(ValueType::eBool, "isDay", IOSocket::eCanHoldBool);
  addOutput(ValueType::eString, "sunrise [str]", IOSocket::eCanHoldString);
  addOutput(ValueType::eString, "sun set [str]", IOSocket::eCanHoldString);

  apply(m_lat,m_lon);
}

void Sunrise::serialize(Json &a_json)
{
  Element::serialize(a_json);
  auto &properties = a_json["properties"];
  properties["lat"] = m_lat.toStdString();
  properties["lon"] = m_lon.toStdString();
}

void Sunrise::deserialize(Json const &a_json)
{
  Element::deserialize(a_json);
  auto const &PROPERTIES = a_json["properties"];
  m_lat = QString::fromStdString(PROPERTIES["lat"].get<std::string>());
  m_lon = QString::fromStdString(PROPERTIES["lon"].get<std::string>());

  apply(m_lat,m_lon);
}

//void Sunrise::update(duration_t const &a_delta)
//{
//}

void Sunrise::calculate()
{
    int r = QDate::currentDate().year();
    int m = QDate::currentDate().month();
    int d = QDate::currentDate().day();
    float lat = m_lat.toFloat();
    float lon = m_lon.toFloat();
    float req = -0.833;

    float j = 367*r-floor(7*(r+floor((m+9)/12))/4)+floor(275*m/9)+d-730531.5;
    float cent = j/36525;

    float l = modd(4.8949504201433+628.331969753199*cent,6.28318530718);
    float g = modd((6.2400408+628.3019501*cent),6.28318530718);
    float o = 0.409093-0.0002269*cent;
    float f = 0.033423*sin(g)+0.00034907*sin(2*g);
    float e = 0.0430398*sin(2*(l+f)) - 0.00092502*sin(4*(l+f)) -f;
    float a = asin(sin(o)*sin(l+f));
    float c = (sin(0.017453293*req) - sin(0.017453293*lat)*sin(a))/(cos(0.017453293*lat)*cos(a));

    float wsch = (3.1415 - (e+0.017453293*lon + 1*acos(c)))*57.29577951/15;
//    float tran = (3.1415 - (e+0.017453293*lon + 0*acos(c)))*57.29577951/15;
    float zach = (3.1415 - (e+0.017453293*lon + (-1*acos(c))))*57.29577951/15;

    QString wschStr = QString::number(ceil(wsch))+":"+QString::number(ceil(60*(wsch-floor(wsch))));
    QString zachStr = QString::number(ceil(zach))+":"+QString::number(ceil(60*(zach-floor(zach))));

    QTime currentTime =  QTime::currentTime();
    QTime wschTime =  QTime(ceil(wsch),ceil(60*(wsch-floor(wsch))));
    QTime zachTime =  QTime(ceil(zach),ceil(60*(wsch-floor(zach))));

    if(currentTime < wschTime)
    {
        m_outputs[1].setValue(false);
    }else
    {
        m_outputs[1].setValue(true);
    }

    m_outputs[2].setValue(wschStr.toStdString());
    m_outputs[3].setValue(zachStr.toStdString());
}

void Sunrise::apply(QString lat, QString lon)
{
    m_lat = lat;
    m_lon = lon;

    m_outputs[0].setValue(true);
}

float Sunrise::acos(float x)
{
    return qAcos(x);
}

float Sunrise::asin(float x)
{
    return qAsin(x);
}

float Sunrise::modd(float x, float y)
{
    float il= floor(x/y);
    return x-(il*y);
}

} // namespace spaghetti::elements
