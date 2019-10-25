FROM debian:latest

RUN apt-get update
RUN apt-get --assume-yes upgrade
RUN apt-get --assume-yes install wget curl make tar git cmake gcc g++
RUN apt-get --assume-yes install libtool pkg-config 

WORKDIR /tmp1
RUN git clone https://github.com/Anastasya83/MRCP-Plugin-Demo.git mrcp
RUN cp -r /tmp1/mrcp/unimrcp-deps-1.5.0/ /usr/local/unimrcp-deps/

RUN cd /usr/local/unimrcp-deps/ && yes | ./build-dep-libs.sh 

RUN cp -r /tmp1/mrcp/unimrcp-1.5.0/ /usr/local/unimrcp/ 

WORKDIR /usr/local/unimrcp/
RUN ./bootstrap
RUN ./configure --with-apr=/usr/local/unimrcp-deps/libs/apr --with-apr-util=/usr/local/unimrcp-deps/libs/apr-util --with-sofia-sip=/usr/local/unimrcp-deps/libs/sofia-sip
#CC=gcc LDFLAGS='-static'
RUN make 
RUN make install

#WORKDIR /usr/local/unimrcp/bin
#CMD ./unimrcpserver
# ./umc

# find . -name "*l.so"
#ln -s /usr/local/apr/lib/libtcnative-1.so /usr/lib/libtcnative-1.so

#export LD_RUN_PATH=/usr/local/lib
#export LD_LIBRARY_PATH=/usr/local/lib/

#echo /opt/program/lib >>/etc/ld.so.conf
# ldconfig
