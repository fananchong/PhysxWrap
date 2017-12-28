FROM centos:centos7.3.1611

RUN rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-7                  && \
    yum install -y gcc gcc-c++ make python-pip initscripts git          && \
    yum install -y libtool-ltdl sudo bzip2.x86_64 which cmake           && \
    yum install -y libstdc++-static glibc-static wget                   && \
    yum clean all
    
RUN wget https://nchc.dl.sourceforge.net/project/p7zip/p7zip/16.02/p7zip_16.02_src_all.tar.bz2  && \
    tar -xvf p7zip_16.02_src_all.tar.bz2                                                        && \
    cd p7zip_16.02                                                                              && \
    make && make install                                                                        && \
    cd .. && rm -rf p7zip_16.02 && rm -rf p7zip_16.02_src_all.tar.bz2
    
VOLUME ["/tmp/home"]