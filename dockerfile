FROM debian:latest

RUN apt-get update
RUN apt-get --assume-yes upgrade
RUN apt-get --assume-yes install wget curl make tar git cmake gcc g++
RUN apt-get --assume-yes install libtool pkg-config 

WORKDIR /tmp9

WORKDIR /tmp
RUN git clone https://github.com/Anastasya83/MRCP-Plugin-Demo.git mrcp 
RUN cp -r /tmp/mrcp/unimrcp-deps-1.5.0/ /usr/local/unimrcp-deps/

RUN cd /usr/local/unimrcp-deps/ && yes | ./build-dep-libs.sh 
RUN cd /usr/local/unimrcp-deps/libs/apr && make install
RUN cd /usr/local/unimrcp-deps/libs/apr-util && make install
RUN cd /usr/local/unimrcp-deps/libs/sofia-sip && make install

RUN cp -r /tmp/mrcp/unimrcp-1.5.0/ /usr/local/unimrcp/ 
ADD Etalons2/ /usr/local/unimrcp/data

WORKDIR /usr/local/unimrcp/
RUN ./bootstrap
RUN ./configure --with-apr=/usr/local/unimrcp-deps/libs/apr --with-apr-util=/usr/local/unimrcp-deps/libs/apr-util --with-sofia-sip=/usr/local/unimrcp-deps/libs/sofia-sip
RUN make && make install

RUN ldconfig -v

WORKDIR /usr/local/unimrcp/bin
CMD ./unimrcpserver
# ./umc

# find . -name "l*.so"
