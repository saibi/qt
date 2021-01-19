#!/bin/bash

cd nex_gui
git checkout -b feature/swissflex origin/feature/swissflex


ARM_CC_BIN=/opt/arm-2009q3/bin
export ARM_CC_BIN
QT_BIN=/opt/qt-4.7.2-nex2/bin
export QT_BIN
PATH=$ARM_CC_BIN:$QT_BIN:$PATH
export PATH

cd new/nex_autoblocker

/opt/qt-4.7.2-nex2/bin/qmake ab8000x.pro

make -j3

