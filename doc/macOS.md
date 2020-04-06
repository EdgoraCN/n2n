# n2n on macOS

In order to use n2n on macOS, you first need to install support for TUN/TAP interfaces:

```bash
#compile
export LDFLAGS="-L/usr/local/opt/openssl/lib" 
export CFLAGS="-I/usr/local/opt/openssl/include" 
export LIBRARY_PATH="/usr/local/opt/openssl/lib:$LIBRARY_PATH"
make clean 
./autogen.sh
./configure
make
otool -L  ./edge
# dependency
brew tap homebrew/cask
brew cask install tuntap
# start
sudo ./edge ./n2n.conf

```

If you are on a modern version of macOS (i.e. Catalina), the commands above will ask you to enable the TUN/TAP kernel extension in System Preferences → Security & Privacy → General.

For more information refer to vendor documentation or the [Apple Technical Note](https://developer.apple.com/library/content/technotes/tn2459/_index.html).
