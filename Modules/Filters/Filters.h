#pragma once

#if 0

    BEGIN_JUCE_MODULE_DECLARATION

      ID:               Filters
      vendor:           MMML
      version:          0.0.1
      name:             Filters
      description:      Filters
      license:          GPL/Commercial
      dependencies:     juce_dsp

    END_JUCE_MODULE_DECLARATION

#endif

#include "Source/HPFilter.h"
#include "Source/LPFilter.h"
#include "Source/PeakFilter.h"