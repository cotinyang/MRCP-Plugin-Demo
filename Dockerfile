FROM debian:latest

RUN apt-get update
RUN apt-get --assume-yes upgrade
RUN apt-get --assume-yes install wget curl make tar git cmake gcc g++ \
                                 libtool pkg-config \
                                 libboost-all-dev \
                                 libghc-zlib-dev \
                                 libghc-bzlib-dev && rm -rf /var/lib/apt/lists/* 

#DATASTORE_LIB
WORKDIR /usr/src
RUN git clone https://github.com/clement/tokyo-cabinet.git tc 
WORKDIR /usr/src/tc
RUN ./configure CC=gcc CFLAGS='-fPIC' CXXFLAGS='-fPIC' && make && make install

#ffmpeg
WORKDIR /usr/src
RUN git clone https://github.com/FFmpeg/FFmpeg.git ffmpeg
WORKDIR /usr/src/ffmpeg
RUN ./configure --disable-x86asm
RUN make && make install

#MRCP src + deps + FFTSS src
WORKDIR /tmp
RUN git clone https://github.com/Anastasya83/MRCP-Plugin-Demo.git mrcp_deps 
RUN mv /tmp/mrcp_deps/unimrcp-deps-1.5.0/ /usr/local/unimrcp-deps/

#FFTSS_LIB
RUN mv /tmp/mrcp_deps/fftss-3.0-20071031/ /usr/src/fftss && chmod -R 755 /usr/src/fftss
WORKDIR /usr/src/fftss
RUN ./configure CC=gcc CFLAGS='-fPIC' CXXFLAGS='-fPIC' && make && make install

RUN cd /usr/local/unimrcp-deps/ && yes | ./build-dep-libs.sh 
RUN cd /usr/local/unimrcp-deps/libs/apr && make install 
RUN cd /usr/local/unimrcp-deps/libs/apr-util && make install 
RUN cd /usr/local/unimrcp-deps/libs/sofia-sip && make install 

#audioneex
WORKDIR /usr/src
RUN git clone https://github.com/Anastasya83/audioneex.git audioneex
WORKDIR /usr/src/audioneex
RUN cp -a /usr/src/fftss/include/ audio/fftss/ && cp -a /usr/src/tc/ DAO/tcabinet/
RUN mkdir build 
WORKDIR /usr/src/audioneex/build
RUN cmake -DDATASTORE_T=TCDataStore .. && make 
RUN cp /usr/src/audioneex/lib/libaudioneex.so /usr/local/lib/ && ldconfig

WORKDIR /tmp
RUN git clone https://github.com/Anastasya83/MRCP-Plugin-Demo.git mrcp 
RUN mv /tmp/mrcp/unimrcp-1.5.0/ /usr/local/unimrcp/ 
RUN mv /tmp/mrcp/Etalons2/ /usr/local/unimrcp/data/
RUN mv /tmp/mrcp/DB/ /usr/local/unimrcp/data/

WORKDIR /usr/local/unimrcp/
RUN ./bootstrap
RUN ./configure --with-apr=/usr/local/unimrcp-deps/libs/apr --with-apr-util=/usr/local/unimrcp-deps/libs/apr-util --with-sofia-sip=/usr/local/unimrcp-deps/libs/sofia-sip
RUN make && make install

RUN ldconfig

WORKDIR /usr/local/unimrcp/bin
CMD ./unimrcpserver
# ./umc
