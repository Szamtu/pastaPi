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

#include <cstdlib>
#include <iostream>

#include <spaghetti/element.h>
#include <spaghetti/logger.h>
#include <spaghetti/registry.h>

#ifdef BUILD_PLUGIN_GUI
#include <spaghettiui/node.h>

#include "dnn_tkdnn/dnn_tkdnn_node.h"
#else
#include <spaghetti/dummynode.h>
#endif

#include "dnn_tkdnn/dnn_tkdnn.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

#ifdef BUILD_PLUGIN_GUI
  a_registry.registerElement<elements::TkDnn, nodes::TkDnn>("tkDNN Yolo", ":/unknown.png");
#else
  a_registry.registerElement<elements::TkDnn>("tkDNN Yolo", ":/unknown.png");
#endif
}
