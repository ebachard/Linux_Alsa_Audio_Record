#!/bin/bash

# this is wav_to_mp3.sh script, to turn an .wav file into .mp3 (please install lamemp3 libs and utils)

# the command is : ./wav_to_mp3 afile.wav

file=$1

ffmpeg -i $file -vn -ar 44100 -ac 2 -b:a 192k ${file%.wav}.mp3



