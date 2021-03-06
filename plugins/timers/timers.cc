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

#include <cstdlib>
#include <iostream>

#include <spaghetti/element.h>
#include <spaghetti/logger.h>
#include <spaghetti/registry.h>

#ifdef BUILD_PLUGIN_GUI
#include <spaghettiui/node.h>


#include "clock/clock_node.h"
#include "sunrise/sunrise_node.h"
#else
#include <spaghetti/dummynode.h>
#endif

#include "clock/clock.h"
#include "delta_time/delta_time.h"
#include "t_off/t_off.h"
#include "t_on/t_on.h"
#include "t_pulse/t_pulse.h"
#include "sunrise/sunrise.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

#ifdef BUILD_PLUGIN_GUI
  a_registry.registerElement<elements::Clock, nodes::Clock>("Clock", ":/unknown.png");
  a_registry.registerElement<elements::Sunrise, nodes::Sunrise>("Sunrise", ":/unknown.png");
#else
  a_registry.registerElement<elements::Clock>("Clock", ":/unknown.png");
  a_registry.registerElement<elements::Sunrise>("Sunrise", ":/unknown.png");
#endif

  a_registry.registerElement<elements::DeltaTime>("Delta time", ":/unknown.png");
  a_registry.registerElement<elements::TimerOff>("Timer OFF", ":/unknown.png");
  a_registry.registerElement<elements::TimerOn>("Timer ON", ":/unknown.png");
  a_registry.registerElement<elements::TimerPulse>("Timer pulse", ":/unknown.png");
}
