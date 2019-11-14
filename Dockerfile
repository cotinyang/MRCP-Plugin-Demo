FROM debian:latest

RUN apt-get update && \
    apt-get --assume-yes upgrade && \
    apt-get --assume-yes install wget curl make tar git cmake gcc g++ \
        libtool pkg-config \
        libboost-all-dev \
        libghc-zlib-dev \
        libghc-bzlib-dev && \
    apt-get --assume-yes clean

# DATASTORE_LIB
RUN git clone https://github.com/clement/tokyo-cabinet.git /usr/src/tc
WORKDIR /usr/src/tc
RUN ./configure CC=gcc CFLAGS='-fPIC' CXXFLAGS='-fPIC' && make && make install

# ffmpeg
RUN git clone https://github.com/FFmpeg/FFmpeg.git /usr/src/ffmpeg
WORKDIR /usr/src/ffmpeg
RUN ./configure --disable-x86asm && make && make install

# MRCP src + deps + FFTSS src
ADD unimrcp-deps-1.5.0 /usr/src/unimrcp-deps/

# FFTSS_LIB
ADD fftss-3.0-20071031 /usr/src/fftss
WORKDIR /usr/src/fftss
RUN ./configure CC=gcc CFLAGS='-fPIC' CXXFLAGS='-fPIC' && make && make install && \
    cd /usr/src/unimrcp-deps/ && yes | ./build-dep-libs.sh && \
    cd /usr/src/unimrcp-deps/libs/apr && make install && \
    cd /usr/src/unimrcp-deps/libs/apr-util && make install && \
    cd /usr/src/unimrcp-deps/libs/sofia-sip && make install

#audioneex
RUN git clone https://github.com/Anastasya83/audioneex.git /usr/src/audioneex
WORKDIR /usr/src/audioneex
RUN cp -a /usr/src/fftss/include/ audio/fftss/ && cp -a /usr/src/tc/ DAO/tcabinet/ && \
    mkdir build
WORKDIR /usr/src/audioneex/build
RUN cmake -DDATASTORE_T=TCDataStore .. && make && \
    cp /usr/src/audioneex/lib/libaudioneex.so /usr/local/lib/ && ldconfig

ADD unimrcp-1.5.0 /usr/src/unimrcp
ADD Etalons2 /usr/local/unimrcp/data
ADD DB/* /usr/local/unimrcp/data/

WORKDIR /usr/src/unimrcp
RUN ./bootstrap && \
    ./configure --with-apr=/usr/src/unimrcp-deps/libs/apr \
        --with-apr-util=/usr/src/unimrcp-deps/libs/apr-util \
        --with-sofia-sip=/usr/src/unimrcp-deps/libs/sofia-sip && \
    make && \
    make install && \
    ldconfig

WORKDIR /

VOLUME /usr/local/unimrcp/conf
CMD /usr/local/unimrcp/bin/unimrcpserver -r /usr/local/unimrcp/ -d -w -o 2 -l 7 && \
    tail -f /usr/local/unimrcp/log/unimrcpserver-00.log
# ./umc

# find . -name "l*.so"