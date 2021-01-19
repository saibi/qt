#!/bin/bash

cd nex_gui
git checkout -b feature/hab910 origin/feature/hab910

cd new
cd cider
/opt/qt-4.7.2-a20/bin/qmake
make -j3 

