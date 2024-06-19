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

#include "DistrhoPluginInfo.h"
#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin to demonstrate parameter outputs using meters.
 */
class MyPlugin : public Plugin {
public:
  MyPlugin()
      : Plugin(1, 0, 0), // 1 parameter, 0 programs, 0 states
        fParam1(0.0) {}

protected:
  /* --------------------------------------------------------------------------------------------------------
   * Information */

  /**
     Get the plugin label.
     A plugin label follows the same rules as Parameter::symbol, with the
     exception that it can start with numbers.
   */
  const char *getLabel() const override { return "dwb"; }

  /**
     Get an extensive comment/description about the plugin.
   */
  const char *getDescription() const override {
    return "Plugin to demonstrate webview ui.";
  }

  /**
     Get the plugin author/maker.
   */
  const char *getMaker() const override { return "dpf webview template"; }

  /**
     Get the plugin homepage.
   */
  const char *getHomePage() const override {
    return "https://github.com/sevenc-nanashi/dpf-webview-template";
  }

  /**
     Get the plugin license name (a single line of text).
     For commercial plugins this should return some short copyright information.
   */
  const char *getLicense() const override { return "MIT"; }

  /**
     Get the plugin version, in hexadecimal.
   */
  uint32_t getVersion() const override { return d_version(1, 0, 0); }

  /* --------------------------------------------------------------------------------------------------------
   * Init */

  /**
     Initialize the audio port @a index.@n
     This function will be called once, shortly after the plugin is created.
   */
  void initAudioPort(bool input, uint32_t index, AudioPort &port) override {
    // treat meter audio ports as stereo
    port.groupId = kPortGroupStereo;

    // everything else is as default
    Plugin::initAudioPort(input, index, port);
  }

  /**
     Initialize the parameter @a index.@n
     This function will be called once, shortly after the plugin is created.
   */
  void initParameter(uint32_t index, Parameter &parameter) override {
    parameter.hints = kParameterIsAutomatable | kParameterIsOutput;
    parameter.name = "Param1";
    parameter.ranges.def = 0.0;
    parameter.ranges.min = 0.0;
    parameter.ranges.max = 100.0;
  }

  /* --------------------------------------------------------------------------------------------------------
   * Internal data */

  /**
     Get the current value of a parameter.
   */
  float getParameterValue(uint32_t index) const override {
    // this is only called for output paramters, and we only have one of those.
    if (index != 0)
      return 0.0;

    return fParam1;
  }

  /**
     Change a parameter value.
   */
  void setParameterValue(uint32_t index, float value) override {
    // this is only called for input paramters, and we only have one of those.
    if (index != 0)
      return;

    fParam1 = value;
  }


  /* --------------------------------------------------------------------------------------------------------
   * Process */

  /**
     Run/process function for plugins without MIDI input.
   */
  void run(const float **inputs, float **outputs, uint32_t frames) override {
  }

  // -------------------------------------------------------------------------------------------------------

private:
  /**
     Parameters.
   */
  float fParam1;

  /**
     Set our plugin class as non-copyable and add a leak detector just in case.
   */
  DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyPlugin)
};

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

Plugin *createPlugin() { return new MyPlugin(); }

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
