#!/bin/bash

cd nex_gui
git checkout -b emw_diagnosis origin/emw_diagnosis 
cd new
cd nex_ibiza
/opt/qt-4.7.2-a20/bin/qmake nex_ibiza.pro
make -j3 
make distclean
/opt/qt-4.7.2-a20/bin/qmake nex_ibiza_d.pro
make -j3 
make distclean
/opt/qt-4.7.2-a20/bin/qmake nex_ibiza_nt.pro
make -j3 
make distclean
/opt/qt-4.7.2-a20/bin/qmake nex_ibiza_ntd.pro
make -j3 

