FROM plengine/build-centos7

RUN curl -o boost_1_64_0.7z -k -L https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.7z
RUN 7za x boost_1_64_0.7z                                                                   
RUN cd boost_1_64_0 && ./bootstrap.sh
RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-program_options
RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-filesystem
RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-system
RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-log
RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-thread
RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-serialization

RUN yum install -y openssl-devel
RUN curl -o /Python-3.6.2.tar.xz -k -L https://www.python.org/ftp/python/3.6.2/Python-3.6.2.tgz
RUN 7za x Python-3.6.2.tar.xz -y -aos -o"." && tar -xvf Python-3.6.2.tar
RUN mkdir -p /python_out && cd /Python-3.6.2 && ./configure --prefix=/python_out && make && make install

RUN sed -i 's/using python : 2.7 : \/usr ;/using python : 3.6 : \/python_out : \/python_out\/include\/python3.6m : \/python_out\/lib ;/g' /boost_1_64_0/project-config.jam

RUN cd boost_1_64_0 && ./b2 stage variant=release threading=multi link=static runtime-link=static address-model=64 --with-python
ADD setup.sh /setup.sh
