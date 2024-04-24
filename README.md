# Easy Embedded Framework

## Description

Easy Embedded (EZ) Framework is an easy-to-use, lightweight, vendor neutral
framework which can be integrated into any embedded system. The framework acts
as the glue code between the application containing the business logics; the
hardware-dependent code such as drivers, startup code; and other third-party
libraries such as RTOS, network stack, file systems. EZ framework provides
a way to seperate between the hardware and the business logics so that both
application developers and firmware developers can concerntrate to do their
job and leave the intagration to the framework.

## How to build and run the project

### Prerequisites

The framework requires the following packages to run on Linux
- CMAKE version 3.25
- doxygen
- graphviz
- gcovr
- make
- gcc
- python3

Those packages can be installed with the following command:

```bash
apt-get install build-essential cmake doxygen gcovr 
```

### General

Since the framework is hardware-independent so it can be built and tested on
PC running Linux or WSL.

To get the framework, users just simply cloning this repository with:

```bash
git clone https://github.com/QuangHaiNguyen/EasyEmbeddedFramework.git
```

Several cmake presets containing different configuration are prepared:

```bash
cmake --list-presets
```

To build one of the preset (I would recommend the linux_threadx_debug), use
the following command:

```bash
cmake --preset=linux_threadx_debug
cmake --build --preset=linux_threadx_debug
```

To run the project, use the following command:

```bash
./build_linux_threadx_debug/targets/linux_threadx/ez_target 
```

If you are using vs code, those commands are integrated provided that you
have the neccessary plugins installed.

### Using Docker

A docker file is prepared so that the users don't have to go through all of
the troubles to start development.

To build the docker image, use the following command:

```bash
make docker_build_image
```

To start the container, use the following command:

```bash
make docker_run_bash
```

Finally, use the folliwng command to get all the supported commands:

```bash
make help
```

### Using github workspace

This project can be opened and run in Github workspace. For more informaion,
please check the following
[file](.devcontainer/devcontainer.json)

## Usage

To integrate the framework to your project, simply add the following command
into your cmake file (provided that the framework is available locally):

```
add_subdirectory(EasyEmbeddedFramework)
```

Then link the framwwork to your project with:

```
target_link_libraries(YOUR_GREAT_PROJECT_NAME PRIVATE easy_embedded_lib)
```

## Running the tests

Test cases are implemented (more will be added in the future) to ensure
the functionality of the framework. To run the test cases (provided that
the linux_test_debug preset is built), use the following command:

```
ctest --preset=linux_test_debug
```

To get the code coverage, use the command:

```
cmake --build --preset=linux_test_debug --target=coverage
```

Last but not list, API document can be get with the command:

```
cmake --build --preset=linux_test_debug --target=doxygen
```

## License

## Roadmap

- Update documents such as system design, architectural design and module design
- Refactor and add more test cases
- Fix non-working components, e.g. cli, rpc
- System test by integrating the framework to target hardware such as pi pico, stm32, esp32
