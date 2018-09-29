# fuck windows et fuck epitech et fuck visual et fuck conan

rm -rf build/
mkdir build

cd build
conan install .. -s arch=x86

sed -i 's/if (NOT CMAKE_CXX_COMPILER_ID MATCHES ${EXPECTED_CMAKE_CXX_COMPILER_ID})/if (NOT WIN32)/g' ./conanbuildinfo.cmake
cmake -G "Visual Studio 15 2017" -T "LLVM-vs2017" ..
cmake --build . --config Release

cp ../Libraries/openssl/windows/lib/libcrypto.dll ./bin/libcryptoMDd.dll
cp ../Libraries/openssl/windows/lib/libssl.dll ./bin/libsslMDd.dll
