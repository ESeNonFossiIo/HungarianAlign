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
rm -rf ./sequence
git add doc
git commit -m "update doc"