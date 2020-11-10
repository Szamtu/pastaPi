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

#pragma once
#ifndef NODES_OPENCV_TKDNN_H
#define NODES_OPENCV_TKDNN_H

#include <spaghettiui/node.h>
#include "dnn_tkdnn.h"

namespace spaghetti::nodes {

class TkDnn : public Node {
 public:
  TkDnn();

 private:
  void refreshCentralWidget() override;
  void showProperties() override;

 private:
  QGraphicsSimpleTextItem *m_info{};
  spaghetti::elements::TkDnn::TkDNNConfiguration m_configuration{};
};

} // namespace spaghetti::nodes

#endif // NODES_OPENCV_TKDNN_H
