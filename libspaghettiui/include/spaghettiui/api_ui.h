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

#pragma once
#ifndef SPAGHETTI_API_UI_H
#define SPAGHETTI_API_UI_H

// clang-format off
#if defined(_WIN64) || defined(_WIN32)
# if defined(SPAGHETTIUI_SHARED)
#  if defined(SPAGHETTIUI_EXPORTS)
#   define SPAGHETTI_API_UI __declspec(dllexport)
#  else
#   define SPAGHETTI_API_UI __declspec(dllimport)
#  endif
# else
#  define SPAGHETTI_API_UI
# endif
#else
# define SPAGHETTI_API_UI __attribute__((visibility("default")))
#endif
// clang-format on

#endif // SPAGHETTI_API_UI_H
