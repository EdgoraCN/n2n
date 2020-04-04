FROM alpine as builder
#ENV http_proxy=http://192.168.2.184:3128
#ENV https_proxy=http://192.168.2.184:3128

RUN  apk add --update --no-cache curl   g++ automake make  autoconf git bash pkgconfig linux-headers  libressl libressl-dev  libpcap libpcap-dev 

RUN  	git clone  https://github.com/aimacity/n2n.git && cd n2n && \
     ./autogen.sh && \
     ./configure && \
     make && mv edge supernode /usr/local/bin

FROM alpine
ARG DEBIAN_FRONTEND=noninteractive
#ENV http_proxy=http://192.168.2.184:3128
#ENV https_proxy=http://192.168.2.184:3128
RUN apk add --update --no-cache curl libressl libpcap && rm -rf /var/lib/apt/lists/* &&  rm -fr /var/cache/apk/*
#ENV http_proxy=
#ENV https_proxy=
COPY --from=builder  /usr/local/bin/supernode /usr/local/bin/edge  /usr/local/bin/

EXPOSE 12151

CMD supernode -l 12151


