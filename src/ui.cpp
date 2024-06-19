/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2024 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoUI.hpp"
#include "MimeTypes/MimeTypes.h"
#include "choc/containers/choc_ZipFile.h"
#include "choc/gui/choc_WebView.h"
#include <istream>
#include <memory>
#include <optional>

#ifndef DEBUG
#include "./ui_zip.h"
#endif

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class MyPluginUi : public UI {

private:
  std::unique_ptr<choc::ui::WebView> chocWebView;
#ifndef DEBUG
  std::shared_ptr<choc::zip::ZipFile> zipFile;
#endif

#ifdef CHOC_WINDOWS
  HWND webViewHwnd;
#else
#error "Unsupported platform"
#endif
public:
  MyPluginUi() : UI() {
    choc::ui::WebView::Options options;
#ifdef DEBUG
    options.enableDebugMode = true;
#else
    options.enableDebugMode = false;

    std::string zipStreamStr(reinterpret_cast<const char *>(UI_ZIP),
                             UI_ZIP_len);
    std::shared_ptr<std::istream> zipStream =
        std::make_shared<std::istringstream>(zipStreamStr);

    zipFile = std::make_shared<choc::zip::ZipFile>(zipStream);

    // Use zip file as resource on release build
    options.fetchResource = [this](const std::string &url)
        -> std::optional<choc::ui::WebView::Options::Resource> {
      for (const auto &item : zipFile->items) {
        if (item.filename == url) {
          auto contentStream = item.createReader();
          std::string content((std::istreambuf_iterator<char>(*contentStream)),
                              std::istreambuf_iterator<char>());
          return std::make_optional<choc::ui::WebView::Options::Resource>(
              content, MimeTypes::getType(url.c_str()));
        }
      }
      return std::nullopt;
    };
#endif

    chocWebView = std::make_unique<choc::ui::WebView>(options);

    // Register callbacks
    chocWebView->bind("hostGetParameter",
                      [this](const choc::value::ValueView &args) {
                        if (args.size() != 1) {
                          return choc::value::Value();
                        }
                        auto index = args[0].getInt64();
                        return choc::value::Value(this->param1);
                      });
    chocWebView->bind(
        "hostSetParameter", [this](const choc::value::ValueView &args) {
          if (args.size() != 2) {
            return choc::value::Value();
          }
          auto index = args[0].getInt64();
          auto value = args[1].isFloat()
                           ? args[1].getFloat64()
                           : static_cast<double>(args[1].getInt64());
          this->setParameterValue(index, value);
          return choc::value::Value();
        });

    // Mount to window
#ifdef CHOC_WINDOWS
    webViewHwnd = static_cast<HWND>(chocWebView->getViewHandle());
    auto windowFlags = GetWindowLongPtr(webViewHwnd, GWL_STYLE);

    windowFlags &= ~WS_POPUP;
    windowFlags |= WS_CHILD;

    ShowWindow(webViewHwnd, SW_HIDE);
    SetWindowLongPtr(webViewHwnd, GWL_STYLE, windowFlags);
    SetParent(webViewHwnd, (HWND)getParentWindowHandle());
    ShowWindow(webViewHwnd, SW_SHOWNA);
#else
#error "Unimplemented"
#endif

#ifdef DEBUG
    // Load development server on debug build
    chocWebView->navigate("http://localhost:5173");
#endif

    setSize(1080, 720);
    onSizeChanged(1080, 720);
  }

  std::uintptr_t getNativeWindowHandle() const noexcept override {
    return reinterpret_cast<std::uintptr_t>(chocWebView->getViewHandle());
  }

  void parameterChanged(uint32_t index, float value) override {
    this->param1 = value;
    this->chocWebView->evaluateJavascript("parameterChanged(" +
                                          std::to_string(index) + "," +
                                          std::to_string(value) + ");");
  }

  void sizeChanged(uint width, uint height) override {
    UI::sizeChanged(width, height);
    onSizeChanged(width, height);
  }

  void onSizeChanged(uint width, uint height) {
#if CHOC_WINDOWS
    if (!SetWindowPos(webViewHwnd, HWND_TOP, 0, 0, width, height,
                      SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER |
                          SWP_NOZORDER)) {
      throw std::runtime_error("Failed to resize window");
    }
#else
#error "Not implemented"
#endif
  }

private:
  float param1 = 0.0f;

  /**
     Set our UI class as non-copyable and add a leak detector just in case.
   */
  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyPluginUi)
};

/* ------------------------------------------------------------------------------------------------------------
 * UI entry point, called by DPF to create a new UI instance. */

UI *createUI() { return new MyPluginUi(); }

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
