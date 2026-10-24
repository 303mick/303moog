// Plain C++ tests for the JUCE-free DSP core. No framework needed; exit code is the result.
#include "../Source/Dsp/Core.h"
#include <cstdio>
#include <cstdlib>

static int failures = 0;
#define CHECK(cond, msg) do { if (! (cond)) { std::printf ("FAIL: %s  (%s)\n", msg, #cond); ++failures; } else std::printf ("ok:   %s\n", msg); } while (0)

static float rms (const std::vector<float>& v, size_t from)
{
    double s = 0; size_t n = 0;
    for (size_t i = from; i < v.size(); ++i) { s += (double) v[i] * v[i]; ++n; }
    return (float) std::sqrt (s / (double) n);
}

static std::vector<float> run (float db, float hz, float freq, int n = 48000)
{
    dsp::Core c;
    c.prepare (48000.0, 1);
    c.setGainDb (db); c.setCutoffHz (hz);
    // let the smoothers settle by running a warm-up block
    std::vector<float> warm ((size_t) 24000, 0.0f); float* wp[] = { warm.data() };
    c.process (wp, 1, (int) warm.size());
    std::vector<float> x ((size_t) n);
    for (int i = 0; i < n; ++i) x[(size_t) i] = std::sin (6.2831853f * freq * (float) i / 48000.0f);
    float* p[] = { x.data() };
    c.process (p, 1, n);
    return x;
}

int main()
{
    { auto y = run (0, 20000, 1000); CHECK (std::fabs (rms (y, 24000) - 0.7071f) < 0.03f, "unity gain passes 1 kHz at ~-3 dB RMS"); }
    { auto y = run (-6, 20000, 1000); CHECK (std::fabs (rms (y, 24000) / 0.7071f - 0.501f) < 0.04f, "-6 dB gain halves the level"); }
    { auto lo = run (0, 500, 100); auto hi = run (0, 500, 8000); CHECK (rms (hi, 24000) < 0.2f * rms (lo, 24000), "low-pass at 500 Hz attenuates 8 kHz by >14 dB vs 100 Hz"); }
    { dsp::Core c; c.prepare (48000, 2); std::vector<float> a (512, 0.0f), b (512, 0.0f); float* p[] = { a.data(), b.data() };
      c.process (p, 2, 512); bool silent = true; for (float v : a) silent &= (v == 0.0f); CHECK (silent, "silence in gives silence out"); }
    { dsp::Core c; c.prepare (48000, 1); c.setGainDb (24); c.setCutoffHz (1e9f); std::vector<float> a (4096, 1.0f); float* p[] = { a.data() };
      c.process (p, 1, 4096); bool finite = true; for (float v : a) finite &= std::isfinite (v); CHECK (finite, "extreme settings stay finite"); }
    { dsp::Core c; c.prepare (48000, 1); c.setGainDb (0); c.setCutoffHz (20000); std::vector<float> a (64, 1e-30f); float* p[] = { a.data() };
      for (int i = 0; i < 2000; ++i)
          c.process (p, 1, 64);
      bool ok = true;
      for (float v : a)
          ok &= std::isfinite (v);
      CHECK (ok, "tiny signals do not blow up"); }
    std::printf ("%s\n", failures ? "TESTS FAILED" : "ALL TESTS PASSED");
    return failures ? 1 : 0;
}
