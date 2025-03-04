#/bin/bash

function install_dep() 
{
    sudo apt update
    sudo apt upgrade
    sudo apt-get install -y sox
    sudo apt install build-essential
    sudo apt install -y libcurl4-openssl-dev cmake
}

function build_whisper_cpp() 
{
    cd whisper.cpp
    cmake -B build
    cmake --build build --config Release

    ./build/bin/whisper-cli -m models/ggml-base.en-q5_1.bin ./samples/jfk.wav 
    if [[ 0 -eq $? ]]; then
        echo "test whisper.cpp err code: $?"
        exit 1
    fi

    cd ..
}

function build_whisper() 
{
    mkdir build
    cd build 
    cmake ..

    make

    cd ..
}

install_dep
build_whisper
build_whisper_cpp
