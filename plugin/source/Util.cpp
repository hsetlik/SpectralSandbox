#include "SpectralSandbox/Audio/Util.h"
#include <cmath>
#include "SpectralSandbox/Common.h"

namespace spec {
constexpr float c_getCenterFrequency(int bin, float sampleRate) {
  return ((float)bin * sampleRate) / (float)FFT_SIZE;
}
// generate our default bin values
constexpr std::array<float, NUM_FFT_BINS> c_defaultBinFrequencies() {
  std::array<float, NUM_FFT_BINS> arr;
  for (int b = 0; b < NUM_FFT_BINS; b++) {
    arr[(size_t)b] = c_getCenterFrequency(b, 44100.0f);
  }
  return arr;
}

// static container for the center frequencies
static std::array<float, NUM_FFT_BINS> binFreqs = c_defaultBinFrequencies();

// update the 'binFreqs' array with the new sample rate
void sampleRateUpdated() {
  const float sr = (float)SampleRate::get();
  for (int b = 0; b < NUM_FFT_BINS; b++) {
    binFreqs[(size_t)b] = ((float)b * sr) / (float)FFT_SIZE;
  }
}
//=============================================================================
static float fBinForHz(float hz) {
  // the basic algebra of this conversion V
  // hz = (bin * sampleRate) / fftSize
  // bin * sampleRate = hz * fftSize
  // bin = (hz * fftSize) / sampleRate
  return (hz * (float)FFT_SIZE) / (float)SampleRate::get();
}

static int closestBinBinarySearch(float hz) {
  // binary search. sinple
  size_t left = 0;
  size_t right = (size_t)NUM_FFT_BINS - 1;
  while (left <= right) {
    size_t mid = left + ((right - left) / 2);
    if (binFreqs[mid] <= hz && hz < binFreqs[mid + 1]) {
      return ((hz - binFreqs[mid]) > (binFreqs[mid + 1] - hz)) ? (int)mid + 1
                                                               : (int)mid;
    } else if (binFreqs[mid] < hz) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return 0;
}

int binForFrequency(float hz) {
#ifndef SEARCH_NEAREST_BIN
  return (int)fBinForHz(hz);
#else
  return closestBinBinarySearch(hz);
#endif
}

float frequencyForBin(int bin) {
  return binFreqs[(size_t)bin];
}

float magAtFrequency(std::complex<float>* data, float hz) {
  // 1. find the bins directly above & below the frequency
  int lowerBin = (int)std::floor(fBinForHz(hz));
  int upperBin = (int)std::ceil(fBinForHz(hz));
  // 2. figure out where it sits between them
  const float t = (hz - binFreqs[(size_t)lowerBin]) /
                  (binFreqs[(size_t)upperBin] - binFreqs[(size_t)lowerBin]);
  // 3. use that proportion to lerp between the magnitudes
  return flerp(std::abs(data[lowerBin]), std::abs(data[upperBin]), t);
}
}  // namespace spec
