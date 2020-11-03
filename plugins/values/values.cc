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

#include "bool/const_bool/const_bool_node.h"
#include "double/const_double/const_double_node.h"
#include "double/random_double/random_double_node.h"
#include "integer/const_int/const_int_node.h"
#include "integer/random_int/random_int_node.h"
#include "string/const_string/const_string_node.h"
#include "values_ui/info_double/info_double_node.h"
#include "values_ui/info_int/info_int_node.h"
#include "values_ui/info_string/info_string_node.h"
#include "values_ui/info_string_vector/info_string_vector_node.h"
#include "values_ui/push_button/push_button_node.h"
#include "values_ui/seven_segment_display/seven_segment_display_node.h"
#include "values_ui/toggle_button/toggle_button_node.h"
#else
#include <spaghetti/dummynode.h>
#endif

#include "bool/const_bool/const_bool.h"
#include "bool/random_bool/random_bool.h"

#include "double/const_double/const_double.h"
#include "double/max_double/max_double.h"
#include "double/min_double/min_double.h"
#include "double/random_double/random_double.h"

#include "integer/const_int/const_int.h"
#include "integer/max_int/max_int.h"
#include "integer/min_int/min_int.h"
#include "integer/random_int/random_int.h"

#include "string/concat_string/concat_string.h"
#include "string/const_string/const_string.h"

#include "convert/convert_value/convert_value.h"
#include "convert/degree_to_radian/degree_to_radian.h"
#include "convert/extract_string/extract_string.h"
#include "convert/radian_to_degree/radian_to_degree.h"

#include "values_ui/info_double/info_double.h"
#include "values_ui/info_int/info_int.h"
#include "values_ui/info_string/info_string.h"
#include "values_ui/info_string_vector/info_string_vector.h"
#include "values_ui/push_button/push_button.h"
#include "values_ui/seven_segment_display/seven_segment_display.h"
#include "values_ui/toggle_button/toggle_button.h"

#include "values_ui/panel/panel.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

#ifdef BUILD_PLUGIN_GUI
  a_registry.registerElement<elements::ConstBool, nodes::ConstBool>("Const bool", ":/unknown.png");

  a_registry.registerElement<elements::ConstDouble, nodes::ConstDouble>("Const float", ":/unknown.png");
  a_registry.registerElement<elements::RandomDouble, nodes::RandomDouble>("Random float", ":/unknown.png");

  a_registry.registerElement<elements::ConstInt, nodes::ConstInt>("Const int", ":/unknown.png");
  a_registry.registerElement<elements::RandomInt, nodes::RandomInt>("Random int", ":/unknown.png");

  a_registry.registerElement<elements::ConstString, nodes::ConstString>("Const string", ":/unknown.png");

  a_registry.registerElement<elements::InfoDouble, nodes::InfoDouble>("Info float", ":/unknown.png");
  a_registry.registerElement<elements::InfoInt, nodes::InfoInt>("Info int", ":/unknown.png");
  a_registry.registerElement<elements::InfoString, nodes::InfoString>("Info string", ":/unknown.png");
  a_registry.registerElement<elements::PushButton, nodes::PushButton>("Push button", ":/unknown.png");
  a_registry.registerElement<elements::SevenSegmentDisplay, nodes::SevenSegmentDisplay>("Seven segment display",
                                                                                        ":/unknown.png");
  a_registry.registerElement<elements::ToggleButton, nodes::ToggleButton>("Toggle button", ":/unknown.png");
  a_registry.registerElement<elements::InfoStringVector, nodes::InfoStringVector>("Info string vector",
                                                                                  ":/unknown.png");
#else
  a_registry.registerElement<elements::ConstBool>("Const bool", ":/unknown.png");

  a_registry.registerElement<elements::ConstDouble>("Const float", ":/unknown.png");
  a_registry.registerElement<elements::RandomDouble>("Random float", ":/unknown.png");

  a_registry.registerElement<elements::ConstInt>("Const int", ":/unknown.png");
  a_registry.registerElement<elements::RandomInt>("Random int", ":/unknown.png");

  a_registry.registerElement<elements::ConstString>("Const string", ":/unknown.png");

  a_registry.registerElement<elements::InfoDouble>("Info float", ":/unknown.png");
  a_registry.registerElement<elements::InfoInt>("Info int", ":/unknown.png");
  a_registry.registerElement<elements::InfoString>("Info string", ":/unknown.png");
  a_registry.registerElement<elements::PushButton>("Push button", ":/unknown.png");
  a_registry.registerElement<elements::SevenSegmentDisplay>("Seven segment display", ":/unknown.png");
  a_registry.registerElement<elements::ToggleButton>("Toggle button", ":/unknown.png");
  a_registry.registerElement<elements::InfoStringVector>("Info string vector", ":/unknown.png");
#endif

  a_registry.registerElement<elements::RandomBool>("Random bool", ":/unknown.png");

  a_registry.registerElement<elements::MinDouble>("Minimum float", ":/unknown.png");
  a_registry.registerElement<elements::MaxDouble>("Maximum float", ":/unknown.png");

  a_registry.registerElement<elements::MinInt>("Minimum int", ":/unknown.png");
  a_registry.registerElement<elements::MaxInt>("Maximum int", ":/unknown.png");

  a_registry.registerElement<elements::ConCat>("Concat string", ":/unknown.png");

  a_registry.registerElement<elements::ConvertValue>("Convert value", ":/unknown.png");
  a_registry.registerElement<elements::Degree2Radian>("Rad2Deg", ":/unknown.png");
  a_registry.registerElement<elements::Radian2Degree>("Deg2Rad", ":/unknown.png");
  a_registry.registerElement<elements::ExtractString>("Extract String", ":/unknown.png");

  a_registry.registerElement<elements::Panel>("Panel", ":/unknown.png");
}
