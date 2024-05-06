FROM debian

# General update
RUN apt-get update -y && apt-get upgrade -y

# build code
RUN apt-get install build-essential cmake --no-install-recommends -y
RUN apt-get install make -y
RUN apt-get install --no-install-recommends curl -y
RUN apt-get install python3 python3-pip -y
RUN apt-get install git -y
RUN apt-get install gcovr -y

# Build API document
RUN apt-get install doxygen graphviz -y

# Build design specification
RUN apt-get install python3-sphinx -y
RUN apt install python3-sphinxcontrib.plantuml

WORKDIR /home/framework