FROM plengine/build-centos7

RUN git clone https://github.com/gperftools/gperftools.git
RUN cd /gperftools && ./autogen.sh && ./configure --enable-libunwind -enable-frame-pointers && make && make install
ADD setup.sh /setup.sh
