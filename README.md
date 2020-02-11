# RestServer
Implementation of simple microservices server

Server configuration:

1. install g++
2. install git
3. sudo apt install cmake
4. clone repo pistache #git clone https://github.com/oktal/pistache.git 
    4.1 Then, init the submodules:

        git submodule update --init

    4.2 Now, compile the sources:

        cd pistache
        mkdir build
        cd build
        cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
        make
        sudo make install

5. Instal openssl libaries > sudo apt-get install libssl-dev

6. git clone https://github.com/arun11299/cpp-jwt.git    to the repo directory

7. visual studio code -> sudo snap install --classic code
 
        sudo /sbin/ldconfig -v

8. PROTOCOL BUFFER:
        sudo apt-get install autoconf automake libtool curl make g++ unzip
        git clone https://github.com/protocolbuffers/protobuf.git
        cd protobuf
        git submodule update --init --recursive
        ./autogen.sh
        ./configure
        make
        make check
        sudo make install
        sudo ldconfig # refresh shared library cache.
	 
	 
	 
	 build command (TODO: cmake):
	 g++ Authorization/Authorization.cpp Controllers/Controller.cpp Database/FakeDB.cpp Services/HelloWorldService.cpp Services/ServiceManager.cpp RestEndpoint.cpp Protocols/generated/sc.pb.cc app.cpp -o restApp -I Authorization -I Controllers -I Database -I Services -I Protocols/generated -I/repo/cpp-jwt/include -I/usr/local/openssl/include -lpistache -lpthread -lcrypto -lssl -lprotobuf