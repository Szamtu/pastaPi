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
#include <spaghetti/node.h>
#include <spaghetti/registry.h>

#include "bool/const_bool/const_bool.h"
#include "bool/const_bool/const_bool_node.h"
#include "bool/random_bool/random_bool.h"

#include "double/const_double/const_double.h"
#include "double/const_double/const_double_node.h"
#include "double/max_double/max_double.h"
#include "double/min_double/min_double.h"
#include "double/random_double/random_double.h"
#include "double/random_double/random_double_node.h"

#include "integer/const_int/const_int.h"
#include "integer/const_int/const_int_node.h"
#include "integer/max_int/max_int.h"
#include "integer/min_int/min_int.h"
#include "integer/random_int/random_int.h"
#include "integer/random_int/random_int_node.h"

#include "string/concat_string/concat_string.h"
#include "string/const_string/const_string.h"
#include "string/const_string/const_string_node.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

  a_registry.registerElement<elements::ConstBool, nodes::ConstBool>("Const bool", ":/unknown.png");
  a_registry.registerElement<elements::RandomBool>("Random bool", ":/unknown.png");

  a_registry.registerElement<elements::ConstDouble, nodes::ConstDouble>("Const float", ":/unknown.png");
  a_registry.registerElement<elements::RandomDouble, nodes::RandomDouble>("Random float", ":/unknown.png");
  a_registry.registerElement<elements::MinDouble>("Minimum float", ":/unknown.png");
  a_registry.registerElement<elements::MaxDouble>("Maximum float", ":/unknown.png");

  a_registry.registerElement<elements::ConstInt, nodes::ConstInt>("Const int", ":/unknown.png");
  a_registry.registerElement<elements::MinInt>("Minimum int", ":/unknown.png");
  a_registry.registerElement<elements::MaxInt>("Maximum int", ":/unknown.png");
  a_registry.registerElement<elements::RandomInt, nodes::RandomInt>("Random int", ":/unknown.png");

  a_registry.registerElement<elements::ConstString, nodes::ConstString>("Const string", ":/unknown.png");
  a_registry.registerElement<elements::ConCat>("Concat string", ":/unknown.png");
}
