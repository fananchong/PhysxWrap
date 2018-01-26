FROM plengine/build-centos7

ADD PhysX-3.4-master.zip /
RUN unzip PhysX-3.4-master.zip
ADD *.mk /PhysX-3.4-master/PhysX_3.4/Source/compiler/linux64/
RUN cd /PhysX-3.4-master/PhysX_3.4/Source/compiler/linux64 && sed -i 's/\#define PX_UNIX_EXPORT __attribute__((visibility(\"default\")))/\#define PX_UNIX_EXPORT/g' /PhysX-3.4-master/PxShared/include/foundation/PxPreprocessor.h
RUN cd /PhysX-3.4-master/PhysX_3.4/Source/compiler/linux64 && sed -i 's/\#define PX_C_EXPORT extern \"C\"/\#define PX_C_EXPORT/g' /PhysX-3.4-master/PxShared/include/foundation/PxPreprocessor.h
RUN cd /PhysX-3.4-master/PhysX_3.4/Source/compiler/linux64 && sed -i 's/\#define PX_ENABLE_DEBUG_VISUALIZATION	1/\#define PX_ENABLE_DEBUG_VISUALIZATION	0/g' /PhysX-3.4-master/PhysX_3.4/Source/Common/src/CmPhysXCommon.h
RUN cd /PhysX-3.4-master/PhysX_3.4/Source/compiler/linux64 && make release
ADD setup.sh /setup.sh
