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

#include "dnn/yolo/yolo_node.h"
#include "ui/video_display/video_display_node.h"
#else
#include <spaghetti/dummynode.h>
#endif

#include "videoio/video_capture/video_capture.h"
#include "videoio/video_writer/video_writer.h"

#include "ui/draw_contours/draw_contours.h"
#include "ui/video_display/video_display.h"

#include "video/mog2/mog2.h"
#include "video/video_bufer/video_bufer.h"

#include "imcodecs/imwrite/imwrite.h"

#include "improc/color2gray/color2gray.h"
#include "improc/crop_image/crop_image.h"
#include "improc/find_conturs/findcontours.h"
#include "improc/medianblur/medianblur.h"
#include "improc/resize/resize.h"

#include "dnn/dnn_draw_labels/dnn_draw_labels.h"
#include "dnn/dnn_threshold/dnn_threshold.h"
#include "dnn/find_class/find_class.h"
#include "dnn/read_classnames/read_classnames.h"
#include "dnn/yolo/yolo.h"

using namespace spaghetti;

extern "C" SPAGHETTI_API void register_plugin(spaghetti::Registry &a_registry)
{
  spaghetti::log::init_from_plugin();

  a_registry.registerElement<elements::VideoCapture>("Video capture", ":/unknown.png");
  a_registry.registerElement<elements::VideoWriter>("Video writer", ":/unknown.png");

#ifdef BUILD_PLUGIN_GUI
  a_registry.registerElement<elements::VideoDisplay, nodes::VideoDisplay>("Video display", ":/unknown.png");
  a_registry.registerElement<elements::Yolo, nodes::Yolo>("YOLOv3", ":/unknown.png");
#else
  a_registry.registerElement<elements::VideoDisplay>("Video display", ":/unknown.png");
  a_registry.registerElement<elements::Yolo>("YOLOv3", ":/unknown.png");
#endif
  a_registry.registerElement<elements::DrawContours>("Draw contours", ":/unknown.png");

  a_registry.registerElement<elements::Mog2>("MOG2", ":/unknown.png");
  a_registry.registerElement<elements::VideoBufer>("Video bufer", ":/unknown.png");

  a_registry.registerElement<elements::ImWrite>("Image write", ":/unknown.png");

  a_registry.registerElement<elements::Color2Gray>("Color to gray", ":/unknown.png");
  a_registry.registerElement<elements::CropImage>("Crop image", ":/unknown.png");
  a_registry.registerElement<elements::FindConturs>("Find contours", ":/unknown.png");
  a_registry.registerElement<elements::MedianBlur>("Median blur", ":/unknown.png");
  a_registry.registerElement<elements::Resize>("Resize image", ":/unknown.png");

  a_registry.registerElement<elements::DnnThreshold>("DNN threshold", ":/unknown.png");
  a_registry.registerElement<elements::ReadClassnames>("Read classnames", ":/unknown.png");
  a_registry.registerElement<elements::DnnDrawLabels>("DNN draw labels", ":/unknown.png");
  a_registry.registerElement<elements::FindClass>("DNN find class", ":/unknown.png");
}
