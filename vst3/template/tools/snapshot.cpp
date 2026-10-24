// Headless native-UI snapshot: renders the real Editor to a PNG with no window or display.
//   snapshot <out.png> [width] [gain01] [cutoff01]
// Lets the QA critic judge the actual JUCE UI the same way it judges the HTML mock.
#include <juce_audio_processors/juce_audio_processors.h>
#include "../Source/PluginProcessor.h"
#include "../Source/PluginEditor.h"
#include "../generated/Params.h"

int main (int argc, char** argv)
{
    juce::ScopedJuceInitialiser_GUI init;
    if (argc < 2) { std::fprintf (stderr, "usage: snapshot out.png [width] [gain01] [cutoff01]\n"); return 2; }

    Processor proc;
    proc.prepareToPlay (48000.0, 512);
    if (argc > 3) proc.apvts.getParameter (params::ids::gain)->setValueNotifyingHost ((float) std::atof (argv[3]));
    if (argc > 4) proc.apvts.getParameter (params::ids::cutoff)->setValueNotifyingHost ((float) std::atof (argv[4]));

    std::unique_ptr<juce::AudioProcessorEditor> ed (proc.createEditor());
    const int w = argc > 2 ? std::atoi (argv[2]) : ed->getWidth();
    ed->setSize (w, (int) std::round (w * (double) ed->getHeight() / ed->getWidth()));
    auto img = ed->createComponentSnapshot (ed->getLocalBounds(), true, 2.0f);   // 2x for crisp review

    juce::File out (juce::File::getCurrentWorkingDirectory().getChildFile (argv[1]));
    out.deleteFile();
    juce::FileOutputStream fos (out);
    if (! fos.openedOk()) { std::fprintf (stderr, "cannot write %s\n", argv[1]); return 1; }
    juce::PNGImageFormat().writeImageToStream (img, fos);
    std::printf ("wrote %s (%dx%d)\n", argv[1], img.getWidth(), img.getHeight());
    return 0;
}
