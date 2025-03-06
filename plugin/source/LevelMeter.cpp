#include "SpectralSandbox/Audio/LevelMeter.h"
#include "SpectralSandbox/Common.h"

float PseudoRMSMeter::process(float input) {
  sum += fastAbs(input);
  ++sampleCount;
  if (sampleCount == windowLength) {
    prevWindowMean = sum / (float)windowLength;
    sum = 0.0f;
    sampleCount = 0;
  }
  return prevWindowMean;
}

float PeakFollower::process(float input) {
  static float prevInput = 0.0f;
  if (std::signbit(input) !=
      std::signbit(prevInput)) {  // we've found a zero crossing
    prevPhasePeak = currentPhasePeak;
    currentPhasePeak = 0.0f;
  }
  float mag = fastAbs(input);
  if (mag > currentPhasePeak)
    currentPhasePeak = mag;
  return prevPhasePeak;
}
