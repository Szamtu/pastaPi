// MIT License
//
// Copyright (c) 2020 Piotr Zdanowicz
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
#include <spaghetti/node.h>
#include <spaghetti/registry.h>

#include "arithmetic/absolute/absolute.h"
#include "arithmetic/addition/addition.h"
#include "arithmetic/division/division.h"
#include "arithmetic/modulo/modulo.h"
#include "arithmetic/multiplication/multiplication.h"
#include "arithmetic/power/power.h"
#include "arithmetic/square/square.h"
#include "arithmetic/subtraction/subtraction.h"

#include "trigonometry/cosinus/cosinus.h"
#include "trigonometry/sinus/sinus.h"
#include "trigonometry/tangens/tangens.h"

#include "rounding/ceil/ceil.h"
#include "rounding/floor/floor.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

  a_registry.registerElement<elements::Absolute>("Absolute", ":/unknown.png");
  a_registry.registerElement<elements::Addition>("Addition", ":/unknown.png");
  a_registry.registerElement<elements::Ceil>("Ceil", ":/unknown.png");
  a_registry.registerElement<elements::Cosinus>("Cosinus", ":/unknown.png");
  a_registry.registerElement<elements::Division>("Division", ":/unknown.png");
  a_registry.registerElement<elements::Floor>("Floor", ":/unknown.png");
  a_registry.registerElement<elements::Modulo>("Modulo", ":/unknown.png");
  a_registry.registerElement<elements::Multiplication>("Multiplication", ":/unknown.png");
  a_registry.registerElement<elements::Power>("Power", ":/unknown.png");
  a_registry.registerElement<elements::Sinus>("Sinus", ":/unknown.png");
  a_registry.registerElement<elements::Square>("Square", ":/unknown.png");
  a_registry.registerElement<elements::Subtraction>("Subtraction", ":/unknown.png");
  a_registry.registerElement<elements::Tangens>("Tangens", ":/unknown.png");
}
