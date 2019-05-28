FROM ubuntu:18.10 as builder

RUN apt-get update && \
	apt-get install -y curl  g++ build-essential net-tools autoconf git && \
	cd $HOME && \
	git clone  https://github.com/aimacity/n2n.git && \
     cd n2n && \
     ./autogen.sh && \
     ./configure && \
     make && mv edge supernode /usr/local/bin && \
     cd  $HOME && rm -fr n2n && \     
	 apt autoremove &&  apt clean  && rm -fr /tmp

FROM ubuntu:18.10
ARG DEBIAN_FRONTEND=noninteractive

# Install the actual VSCode to download configs and extensions
RUN apt-get update && \
	apt-get install -y  net-tools  && \
	 apt autoremove &&  apt clean  && rm -fr /tmp/*

COPY --from=builder  /usr/local/bin/supernode /usr/local/bin/edge  /usr/local/bin/

EXPOSE 12151

CMD edge -l 12151


