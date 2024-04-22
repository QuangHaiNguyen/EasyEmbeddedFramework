FROM debian

# General update
RUN apt-get update -y && apt-get upgrade -y

# build tool
RUN apt-get install build-essential cmake --no-install-recommends -y
RUN apt-get install gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib -y
RUN apt-get install make -y
RUN apt-get install doxygen graphviz -y
RUN apt-get install --no-install-recommends curl -y
RUN apt-get install python3 python3-pip -y
RUN apt-get install git -y
RUN apt-get install gcovr -y

WORKDIR /home/framework