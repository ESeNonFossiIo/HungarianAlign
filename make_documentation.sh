#!/bin/bash

git clone https://github.com/ESeNonFossiIo/sequence.git
cd sequence
mkdir build
cd build
cmake ..
make doc
cd ../../
rm -rf ./doc
mv sequence/doc .