#pragma once
/* This file just includes juce and sets up a few
 * typedefs that I find handy in every juce project
 * */
#include "Identifiers.h"
#include "juce_audio_processors/juce_audio_processors.h"
constexpr float twoPi_f = juce::MathConstants<float>::twoPi;
//
// maybe we'll avoid writing 5 zillion 'prepare()' functions this way
namespace SampleRate {
void set(double rate);
double get();
}  // namespace SampleRate

// misc. handy math stuff
inline float flerp(float a, float b, float t) {
  return a + ((b - a) * t);
}

// this function simply clears the MSB of the value that represents its sign
inline float fastAbs(float& val) {
  static constexpr int mask = ~(1 << 31);
  int bits = mask & *reinterpret_cast<int*>(&val);
  return *reinterpret_cast<float*>(&bits);
}

//=======================================================================
#define FFT_ORDER 10
constexpr int FFT_SIZE = 1 << FFT_ORDER;        // 1024 samples
constexpr int NUM_FFT_BINS = FFT_SIZE / 2 + 1;  // 513 bins
