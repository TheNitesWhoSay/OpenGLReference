FROM ubuntu:rolling

RUN apt-get update
RUN apt-get install wget lsb-release software-properties-common gnupg -y dialog
RUN wget https://apt.llvm.org/llvm.sh
RUN chmod u+x llvm.sh
RUN ./llvm.sh 19
RUN ln -s /bin/clang-19 /bin/clang
RUN ln -s /bin/clang++-19 /bin/clang++
ENV CC=clang
ENV CXX=clang++
RUN apt-get install cmake curl zip git ninja-build -y dialog
RUN ln -s /usr/bin/ninja /usr/bin/ninja-build
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg
RUN cd /opt/vcpkg && ./bootstrap-vcpkg.sh -useSystemBinaries && ./vcpkg integrate install
ENV CMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
ENV VCPKG_ROOT=/opt/vcpkg
# Note that a real linux instance might also want: sudo chown -R $USER:$USER /opt/vcpkg

#Dependencies related to ICU/GLFW/OpenGL
RUN apt-get install pkg-config autoconf automake autoconf-archive -y dialog
RUN apt-get install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev -y dialog

# Clone and build this project
RUN git clone https://github.com/TheNitesWhoSay/OpenGLReference.git ~/OpenGLReference
RUN mkdir ~/OpenGLReference/cbuild
RUN cd ~/OpenGLReference/cbuild && cmake -G "Ninja" ../ && cmake --build .

## Notes (/end of docker build file) ##

# Build and run:
#docker build -t linuxclang -m4GB .
#docker run -it linuxclang

# List containers
#docker container ls --all

# Delete container
#docker container rm 12ab34f

# Restart/reattach console to container:
#docker start 12ab34f
#docker attach 12ab34f
