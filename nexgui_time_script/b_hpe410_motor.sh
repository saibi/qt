#!/bin/bash
cd autocrat
git checkout -b motor_v1.11.0_emw origin/motor_v1.11.0_emw 
export ARM_NONE_EABI_BIN_AUTOCRAT=/opt/arm-2013.05/bin
make h=i RC=1 -j2
