#pragma once

#include "Util.h"

// this provides identical response to an RMS meter
// thansk to a little bitmask trick
class PseudoRMSMeter {
private:
  float prevWindowMean = 0.0f;
  float sum = 0.0f;
  int sampleCount = 0;
  int windowLength = 512;

public:
  PseudoRMSMeter() = default;
  int getWindow() const { return windowLength; }
  void setWindow(int length) { windowLength = length; }
  float process(float input);
};

//================================================

class PeakFollower {
private:
  float prevPhasePeak = 0.0f;
  float currentPhasePeak = 0.0f;

public:
  PeakFollower() = default;
  float process(float input);
};
