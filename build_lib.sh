#!/bin/sh
g++ -fpic -shared -o libPlayer.so player.cpp `pkg-config --cflags gstreamer-1.0`
