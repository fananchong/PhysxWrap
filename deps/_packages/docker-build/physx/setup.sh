#!/bin/bash

set -e

cp -f /PhysX-3.4-master/PhysX_3.4/Lib/linux64/*.a /tmp/home/lib_linux
cp -f /PhysX-3.4-master/PxShared/lib/linux64/*.a /tmp/home/lib_linux
cp -f /PhysX-3.4-master/Externals/nvToolsExt/1/lib/armv7/*.a /tmp/home/lib_linux
cp -f -r /PhysX-3.4-master/PhysX_3.4/Include/* /tmp/home/include/physx/PhysX_3.4/
cp -f -r /PhysX-3.4-master/PxShared/include/* /tmp/home/include/physx/PxShared/

