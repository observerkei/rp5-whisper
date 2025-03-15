# 功能

语音转文字封装

- 支持离线语言识别成文字
- 支持设定环境音，超过环境音才进行识别
- 通过使用系统 arecord/sox 提高兼容性，语音支持按秒分片处理，自动合并分片
- 支持预设识别映射，可通过配置文件进行调整，用于提高识别精确度

# 环境准备

## 配置依赖

bash -x ./build_environment.sh

## 构建 whisper.cpp 服务

```bash
cd whisper.cpp
cmake -B build
cmake --build build --config Release

# 测试编译是否成功
./build/bin/whisper-cli -m models/ggml-base.en-q5_1.bin ./samples/jfk.wav 

# 返回上层目录
cd ..
```

## 编译项目

```bash
mkdir build
cd build 
cmake ..
make
```

# 修改配置

## 识别配置

配置是一个数组，每个数组是一组识别关系，语音录制音频后，会转化为 text 文字，  
然后如果text文字在 config.json 中，那么就会转化为对应的 code 进行输出. 

```json
[
    {
        "text": [
            "hi."
        ],
        "code": "0x01"
    },
    {
        "text": [
            "up.",
            "up?",
            "oh.",
            "ow.",
            "ah.",
            "wow."
        ],
        "code": "0x02"
    },
    {
        "text": [
            "How are you?",
            "okay.",
            "okay",
            "okay?",
            "okay!"
        ],
        "code": "0x03"
    }
]
```

## 识别模型

查看模型

```bash
cd whisper.cpp/models/ 
./download-ggml-model.sh 
```

下载模型

```bash
./download-ggml-model.sh  base.en-q5_1
```

使用不同模型， 修改 `start_whisper_server.sh`, 把 -m 修改为想要的模型即可。

```bash
cd whisper.cpp && ./build/bin/whisper-server --port 8090 -m models/ggml-base.en-q5_1.bin
```

## 环境音大小

在 `./src/whisper.h` 的 `SILENCE_THRESHOLD` 中可以配置环境音大小，用于区分审核时候有人说话。


# 执行例程 `src/main.cpp -> whisper`

## 启动 whisper.cpp 服务

```bash
./start_whisper_server.sh
```

## 启动 whisper 例程

```bash
cd build
./whisper ../config.json
```

先说 up 过会再说 okay，会有如下日志

```bash
 rp5-whisper/build main SIGINT ❯ ./whisper ../config.json
[rp5-whisper/whisper.cpp:read_config:45] read hi -> 0x01
[rp5-whisper/whisper.cpp:read_config:45] read up -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read up. -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read up? -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read oh. -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read ow. -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read ah. -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read wow. -> 0x02
[rp5-whisper/whisper.cpp:read_config:45] read how are you? -> 0x03
[rp5-whisper/whisper.cpp:read_config:45] read okay. -> 0x03
[rp5-whisper/whisper.cpp:read_config:45] read okay -> 0x03
[rp5-whisper/whisper.cpp:read_config:45] read okay? -> 0x03
[rp5-whisper/whisper.cpp:read_config:45] read okay! -> 0x03
Task whisper is running on thread 139692007523904
#  +                                               | 05%Maximum peak loudness (dB): -24.5799 dB
[rp5-whisper/whisper.cpp:whisper:174] max_loudness(-24.579869) > SILENCE_THRESHOLD(-30), continue record
[rp5-whisper/main.cpp:whisper_task:22] whisper ret: 1
Recording WAVE '/tmp/record_1.wav' : Signed 16 bit Little Endian, Rate 16000 Hz, Mono
#+                                                 | 00%Maximum peak loudness (dB): -49.2474 dB
[rp5-whisper/whisper.cpp:voice_to_text:89] Text: Wow.
[rp5-whisper/main.cpp:whisper_callback:10] [6] get code: 0x02
[rp5-whisper/main.cpp:whisper_task:22] whisper ret: 0
Recording WAVE '/tmp/record_0.wav' : Signed 16 bit Little Endian, Rate 16000 Hz, Mono
#+                                                 | 00%Maximum peak loudness (dB): -48.3016 dB
[rp5-whisper/whisper.cpp:whisper:166] max_loudness(-48.301588) <= SILENCE_THRESHOLD(-30), no voice
[rp5-whisper/main.cpp:whisper_task:22] whisper ret: -2
Recording WAVE '/tmp/record_0.wav' : Signed 16 bit Little Endian, Rate 16000 Hz, Mono
#     +                                            | 11%Maximum peak loudness (dB): -18.4721 dB
[rp5-whisper/whisper.cpp:whisper:174] max_loudness(-18.472130) > SILENCE_THRESHOLD(-30), continue record
[rp5-whisper/main.cpp:whisper_task:22] whisper ret: 1
Recording WAVE '/tmp/record_1.wav' : Signed 16 bit Little Endian, Rate 16000 Hz, Mono
#+                                                 | 00%Maximum peak loudness (dB): -49.2474 dB
[rp5-whisper/whisper.cpp:voice_to_text:89] Text: Okay.
[rp5-whisper/main.cpp:whisper_callback:10] [13] get code: 0x03
[rp5-whisper/main.cpp:whisper_task:22] whisper ret: 0
```

up 转化为了 0x02, okay 转化为 0x03

