#se.en-q5_1.bin!/bin/bash

MODEL=base.en-q5_1
MODEL_PATH=models/ggml-${MODEL}.bin

if [[ ! -f ${MODEL} ]]; then
    cd whisper.cpp/models && ./download-ggml-model.sh ${MODEL} && cd -
fi

cd whisper.cpp && ./build/bin/whisper-server --port 8090 -m ${MODEL_PATH}
