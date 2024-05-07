#!/bin/bash
set -eu

## Setup python venv
echo "Setting up python venv..."
python3 -m venv .venv

## Activeate
echo "Activating python venv..."
. .venv/bin/activate

## Install packages
echo "Installing python packages..."
pip install -r requirements.txt

## This is practice
echo "Building C object using cmake..."
mkdir -p build
cd build
cmake ..
make

echo "Setup complete!"
echo "The object file is placed in ./build/"
echo "First, activate the venv (source .venv/bin/activate)."

echo "Next, run \"./build/main_pkt inputdata time verbose\"."
echo "         (For example, ./build/main_pkt \"test.pcap\" 1 0)"
echo "Finally, run \"python plot1.py -t time -v 0or1\" to get result."
echo "         (For example, python plot1.py -t 1 -v 0)"