cp -fr /lib/x86_64-linux-gnu/libcrypt.so.1.1.0  libcrypt.so
cp -fr edge edge-agent-linux
tar zcvf edge-agent.linux.tar.gz edge-agent-linux scripts libcrypt.so