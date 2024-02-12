FROM ubuntu

# General update
RUN apt-get update -y && apt-get upgrade -y

# build tool
RUN apt-get install build-essential cmake --no-install-recommends -y
RUN apt-get install make -y
RUN apt-get install ninja-build doxygen graphviz -y
RUN apt-get install --no-install-recommends curl -y
RUN apt-get install python3 python3-pip -y
RUN apt-get install git -y

WORKDIR /home/framework