.PHONY: help \
	docker_build_image \
	docker_run_bash \
	docker_build_clean \
	docker_build \
	docker_run_tests

docker_build_image:
	docker build . --tag toolchains

docker_run_bash:
	docker run --mount src=".",target=/home/framework,type=bind -it --rm toolchains

docker_build_clean:
	docker run \
		--mount src=".",target=/home/framework,type=bind \
		--rm toolchains \
		rm -rf build; \
		mkdir build; \
		cmake -S . -B build; \
		cmake --build build

docker_build:
	docker run \
		--mount src=".",target=/home/framework,type=bind \
		--rm toolchains \
		cmake -S . -B build; \
		cmake --build build

docker_run_tests:
	docker run \
		--mount src=".",target=/home/framework,type=bind \
		--rm toolchains \
		./build/tests/ez_unit_test
help:
	$(info lists of targets:)
	$(info docker_build_image: build docker image based on the dockerfile)
	$(info docker_run_bash: run container in interactive mode)
	$(info docker_build_clean: Clean build of the software)
	$(info docker_build: build the software, provided that the build folder is available)
	$(info docker_run_tests: run available unit test)
	$(info help: print this message)