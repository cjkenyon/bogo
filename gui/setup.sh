#!/usr/bin/bash

echo "STARTING STREAM"
./mk_stream.sh &
echo "STARING CAMERA"
source ~/gui/venv/bin/activate
gnome-terminal --geometry=100x10 -- bash -c "echo \"STARTING GUI\"; source venv/bin/activate; python3 ./gui.py"
python3 ./cam.py &
ssh t4@rpi.local

