FROM debian:latest

RUN apt-get update && \
    apt-get --assume-yes upgrade && \
    apt-get --assume-yes install wget curl tar git cmake build-essential \
        libtool pkg-config libboost-all-dev libghc-zlib-dev libghc-bzlib-dev \
        libavcodec-dev libavdevice-dev libavfilter-dev libavformat-dev libavresample-dev libavutil-dev libpostproc-dev \
        libswresample-dev libswscale-dev libtokyocabinet-dev && \
    apt-get --assume-yes clean

# DATASTORE_LIB
ADD tc /usr/src/tc
#RUN cd /usr/src/tc && \
#    ./configure CC=gcc CFLAGS='-fPIC' CXXFLAGS='-fPIC' && \
#    make && \
#    make install

# FFTSS_LIB
ADD fftss-3.0-20071031 /usr/src/fftss
RUN cd /usr/src/fftss && \
    ./configure CC=gcc CFLAGS='-fPIC' CXXFLAGS='-fPIC' && \
    make && \
    make install

# MRCP deps
ADD unimrcp-deps-1.5.0 /usr/src/unimrcp-deps/
RUN cd /usr/src/unimrcp-deps/ && yes | ./build-dep-libs.sh && \
    cd /usr/src/unimrcp-deps/libs/apr && make install && \
    cd /usr/src/unimrcp-deps/libs/apr-util && make install && \
    cd /usr/src/unimrcp-deps/libs/sofia-sip && make install

#audioneex
ADD audioneex /usr/src/audioneex
RUN cp -a /usr/src/fftss/include/ /usr/src/audioneex/audio/fftss/ && \
    cp -a /usr/src/tc/ /usr/src/audioneex/DAO/tcabinet/ && \
    mkdir /usr/src/audioneex/build && \
    cd /usr/src/audioneex/build && \
    cmake -DDATASTORE_T=TCDataStore .. && \
    make && \
    cp /usr/src/audioneex/lib/libaudioneex.so /usr/local/lib/ && \
    ldconfig

ADD unimrcp-1.5.0 /usr/src/unimrcp
ADD Etalons2 /usr/local/unimrcp/data
ADD DB /usr/local/unimrcp/data/DB

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
CMD /usr/local/unimrcp/bin/unimrcpserver -r /usr/local/unimrcp/ -w -o 1 -l 7
#    tail -f /dev/null