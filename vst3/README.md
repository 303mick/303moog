# VST3 Builder

Takes a plugin from idea to a designed, tested, buildable VST3. Start it in Claude Code with:

    /vst3-builder <plugin idea>

Three agents do the work: **Agent 1 UI Design**, **Agent 2 UX Design**, **Agent 3 Builder** (see `VST3_BUILDER.md`).

| Path | What it is |
|---|---|
| `template/` | Working JUCE 8 plugin skeleton: tested DSP core, tokens-driven editor, snapshot tool |
| `tools/vst3_gen.py` | `plugin.json` + `tokens.json` -> `Params.h` + `Tokens.h` |
| `CMakeLists.txt` | `-DPLUGIN_DIR=template` (or `plugins/<slug>`), optional `-DJUCE_SOURCE_DIR=` |
| `../.github/workflows/vst3-build.yml` | Windows VST3 build + pluginval, Linux tests + native snapshots |

Local build (Linux or Windows with CMake):

    cmake -S vst3 -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j4
    ctest --test-dir build

Install on Windows: download the `vst3-windows` artifact from the Actions run, copy the `.vst3` folder to `C:\Program Files\Common Files\VST3\`, rescan in Ableton.
