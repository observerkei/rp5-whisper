# whisper.cpp/examples/fuzzy_stream

This is a naive example of performing real-time inference on audio from your microphone.
The `whisper-fuzzy` tool samples the audio every half a second and runs the transcription continously.

```bash
./build/bin/whisper-fuzzy -u config.json -m ./models/ggml-base.en.bin -t 8 --step 500 --length 5000
```

## Sliding window mode with VAD

Setting the `--step` argument to `0` enables the sliding window mode:

```bash
 ./build/bin/whisper-fuzzy -u config.json -m ./models/ggml-base.en.bin -t 6 --step 0 --length 5000 -vth 0.6
```

In this mode, the tool will transcribe only after some speech activity is detected. A very
basic VAD detector is used, but in theory a more sophisticated approach can be added. The
`-vth` argument determines the VAD threshold - higher values will make it detect silence more often.
It's best to tune it to the specific use case, but a value around `0.6` should be OK in general.
When silence is detected, it will transcribe the last `--length` milliseconds of audio and output
a transcription block that is suitable for parsing.

## Building

The `whisper-fuzzy` tool depends on SDL2 library to capture audio from the microphone. You can build it like this:

```bash
# Install SDL2
# On Debian based linux distributions:
sudo apt-get install libsdl2-dev

# On Fedora Linux:
sudo dnf install SDL2 SDL2-devel

# Install SDL2 on Mac OS
brew install sdl2

cd ..
bash -x build_environment.sh
cmake -B build -DWHISPER_SDL2=ON
cmake --build build --config Release

./build/bin/whisper-fuzzy
```

