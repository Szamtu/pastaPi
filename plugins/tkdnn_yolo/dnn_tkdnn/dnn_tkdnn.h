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
#ifndef SPAGHETTI_ELEMENTS_OPENCV_TKDNN_H
#define SPAGHETTI_ELEMENTS_OPENCV_TKDNN_H

#include <spaghetti/element.h>
#include <tkDNN/Yolo3Detection.h>
#include <tkDNN/tkdnn.h>

namespace spaghetti::elements {

class TkDnn final : public Element {
 public:
  static constexpr char const *const TYPE{ "OpenCV/DNN/tkdnn" };
  static constexpr string::hash_t const HASH{ string::hash(TYPE) };
  bool alwaysCalculate() const noexcept override { return false; }

  TkDnn();

  char const *type() const noexcept override { return TYPE; }
  string::hash_t hash() const noexcept override { return HASH; }

  void calculate() override;

  void serialize(Json &a_json) override;
  void deserialize(Json const &a_json) override;

  struct TkDNNConfiguration {
    std::string networkFilePath{};
    int classes{ 80 };
    double confidenceThreshold{ 0.2 };
    bool syncInputs{ false };
  };

  bool confugure(TkDNNConfiguration const a_configuration);
  TkDNNConfiguration configuration() { return m_configuration; }

 private:
  TkDNNConfiguration m_configuration{};
  tk::dnn::Yolo3Detection m_yoloNetwork;
  bool m_networkReaddy{};
  std::mutex m_lock{};
};

} // namespace spaghetti::elements

#endif // SPAGHETTI_ELEMENTS_OPENCV_TKDNN_H
