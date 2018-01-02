FROM plengine/build-centos7



ADD PhysX-3.4-master.zip /
RUN unzip PhysX-3.4-master.zip
RUN cd /PhysX-3.4-master/PhysX_3.4/Source/compiler/linux64 && make
ADD setup.sh /setup.sh
