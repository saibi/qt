#!/bin/bash

cd nex_gui
git checkout -b release/edger_v1.12.0_910 origin/release/edger_v1.12.0_910 
cd new
cd wedge
echo "N" > model.910
/opt/qt-4.7.2-a20/bin/qmake
make -j3
make distclean
echo "D" > model.910
/opt/qt-4.7.2-a20/bin/qmake
make -j3
make distclean
echo "X" > model.910
/opt/qt-4.7.2-a20/bin/qmake
make -j3
make distclean
echo "XD" > model.910
/opt/qt-4.7.2-a20/bin/qmake
make -j3

