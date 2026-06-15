# Version: 2.1.0
FROM centos:7

RUN sed -i 's/mirrorlist/#mirrorlist/g' /etc/yum.repos.d/CentOS-*.repo && \
    sed -i 's|#baseurl=http://mirror.centos.org|baseurl=http://vault.centos.org|g' /etc/yum.repos.d/CentOS-*.repo && \
    yum update -y && \
    yum install -y epel-release && \
    yum install -y qt5-qtbase-devel gcc-c++ make && \
    yum clean all

WORKDIR /usr/src/game_server

COPY server/ .

RUN /usr/lib64/qt5/bin/qmake && make

EXPOSE 33333

CMD ["./server"]
