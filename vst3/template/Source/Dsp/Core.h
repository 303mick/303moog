#pragma once
// JUCE-free DSP core. Everything here must be real-time safe:
// no allocation, no locks, no I/O in process(). Allocate in prepare().
#include <algorithm>
#include <cmath>
#include <vector>

namespace dsp
{
inline float dbToLinear (float db) { return std::pow (10.0f, db * 0.05f); }

// One-pole smoother to avoid zipper noise on parameter changes.
struct Smoothed
{
    void reset (double sampleRate, float timeSeconds, float initial)
    {
        coeff = 1.0f - std::exp (-1.0f / (float) (sampleRate * timeSeconds));
        value = target = initial;
    }
    void set (float t) { target = t; }
    float next() { value += coeff * (target - value); return value; }
    float coeff = 0.01f, value = 0.0f, target = 0.0f;
};

class Core
{
public:
    void prepare (double sr, int channels)
    {
        sampleRate = sr;
        lp.assign ((size_t) channels, 0.0f);
        gain.reset (sr, 0.02f, gain.value);
        cutoff.reset (sr, 0.02f, cutoff.value);
    }

    void setGainDb (float db)   { gain.set (dbToLinear (db)); }
    void setCutoffHz (float hz) { cutoff.set (std::clamp (hz, 10.0f, (float) (sampleRate * 0.45))); }

    // In-place processing of non-interleaved channels.
    void process (float* const* ch, int numCh, int n)
    {
        for (int i = 0; i < n; ++i)
        {
            const float g = gain.next();
            const float c = cutoff.next();
            const float a = 1.0f - std::exp (-6.28318530718f * c / (float) sampleRate);
            for (int k = 0; k < numCh; ++k)
            {
                float& z = lp[(size_t) k];
                z += a * (ch[k][i] - z);
                ch[k][i] = z * g;
            }
        }
    }

private:
    double sampleRate = 44100.0;
    std::vector<float> lp;
    Smoothed gain { 0.02f, 1.0f, 1.0f };
    Smoothed cutoff { 0.02f, 20000.0f, 20000.0f };
};
} // namespace dsp
