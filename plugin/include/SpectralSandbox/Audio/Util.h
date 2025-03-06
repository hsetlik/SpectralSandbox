#pragma once

#include "../Common.h"
#include <juce_dsp/juce_dsp.h>

// Spectral math stuff ========================================================
namespace spec {
// get the nearest FFT bin index for a given frequency in hz
int binForFrequency(float hz);
// get the center frequency of the FFT bin at the given index
float frequencyForBin(int bin);
// call this when the sample rate changes to keep the frequency/bin index
// conversion as accurate as possible
void sampleRateUpdated();
// get the spectrum's magnitude at the given frequency
float magAtFrequency(std::complex<float>* data, float hz);
}  // namespace spec
