#  Compilation for iOS

1. move source files from resiprocate/rutil/dns to resiprocate/contrib/ares
2. change Base sdk from macOS to iOS latest
3. fix 'openssl/evp.h' file not found by adding headers search path for OpenSSL
- Download https://github.com/x2on/OpenSSL-for-iPhone zip archive and copy to resiprocate folder.
- ./build-libssl.sh --version=1.1.0i --archs="x86_64 i386 arm64 armv7s armv7"
4. Project settings -> resiprocate target settings -> Build phases -> Link binary with libraries
Add libssl.a from resiprocate/openssl/lib
5. Update User Header search paths to:
../openssl/include

