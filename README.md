# 功能

语音转文字封装

- 支持离线语言识别成文字
- 通过流式处理语音，支持限制识别时长等功能
- 支持预设识别映射，可通过配置文件进行调整，用于提高识别精确度
- 其他未说明功能请看 `查看帮助` 一栏


# 环境准备

## 配置依赖

bash -x ./build_environment.sh

## 构建 whisper.cpp 服务

```bash
cmake -B build -DWHISPER_SDL2=ON -DENABLE_GDB=OFF -S whisper.cpp
cmake --build build --config Release

# 测试编译是否成功
./build/bin/whisper-cli -m ./whisper.cpp/models/ggml-base.en-q5_1.bin ./whisper.cpp/samples/jfk.wav 

# 返回上层目录
cd ..
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

使用不同模型， 修改 启动参数, 把 -m 修改为想要的模型即可。

```bash
./build/bin/whisper-fuzzy -u ./config.json -m ./whisper.cpp/models/ggml-base.en-q5_1.bin -t 6 --step 0 --length 3000 -vth 0.6
```


# 执行例程 `src/main.cpp -> build/bin/whisper-fuzzy`

示例程序 `src/main.cpp`

## 查看帮助

```bash
./build/bin/whisper-fuzzy
```

## 启动 whisper 例程

```bash
./build/bin/whisper-fuzzy -u ./config.json -m ./whisper.cpp/models/ggml-base.en-q5_1.bin -t 6 --step 0 --length 3000 -vth 0.6
```

先说 okey 过会再说 oh，会有如下日志

```bash
 rp5-whisper/build main SIGINT ❯ ./build/bin/whisper-fuzzy -u ./config.json -m ./whisper.cpp/models/ggml-base.en-q5_1.bin -t 6 --step 0 --length 5000 -vth 0.6 
[/whisper.cpp:read_config:45] read hi -> 0x01
[/whisper.cpp:read_config:45] read up -> 0x02
[/whisper.cpp:read_config:45] read up. -> 0x02
[/whisper.cpp:read_config:45] read up? -> 0x02
[/whisper.cpp:read_config:45] read oh. -> 0x02
[/whisper.cpp:read_config:45] read ow. -> 0x02
[/whisper.cpp:read_config:45] read ah. -> 0x02
[/whisper.cpp:read_config:45] read wow. -> 0x02
[/whisper.cpp:read_config:45] read how are you? -> 0x03
[/whisper.cpp:read_config:45] read okay. -> 0x03
[/whisper.cpp:read_config:45] read okay -> 0x03
[/whisper.cpp:read_config:45] read okay? -> 0x03
[/whisper.cpp:read_config:45] read okay! -> 0x03
[/whisper_stream.cpp:whisper_stream_main:283]
[/whisper_stream.cpp:whisper_stream_main:284] ### Transcription 11 START | t0 = 82625 ms | t1 = 112625 ms
[/whisper_stream.cpp:whisper_stream_main:285]
[/main.cpp:whisper_user_callback:29] [11] get text:  Okay., code: 0x03
[/whisper_stream.cpp:whisper_stream_main:313] [00:00:02.000 --> 00:00:29.600]   Okay.
[/whisper_stream.cpp:whisper_stream_main:327]
[/whisper_stream.cpp:whisper_stream_main:328] ### Transcription 11 END
[/whisper_stream.cpp:whisper_stream_main:283]
[/whisper_stream.cpp:whisper_stream_main:284] ### Transcription 12 START | t0 = 92956 ms | t1 = 122956 ms
[/whisper_stream.cpp:whisper_stream_main:285]
[/whisper_stream.cpp:whisper_stream_main:313] [00:00:00.000 --> 00:00:20.560]   Okay.
[/main.cpp:whisper_user_callback:29] [12] get text:  Oh., code: 0x02
[/whisper_stream.cpp:whisper_stream_main:313] [00:00:20.560 --> 00:00:29.000]   Oh.
[/whisper_stream.cpp:whisper_stream_main:327]
[/whisper_stream.cpp:whisper_stream_main:328] ### Transcription 12 END
```

okay 转化为 0x03, oh 转化为了 0x02

