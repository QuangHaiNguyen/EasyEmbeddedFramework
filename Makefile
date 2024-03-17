.PHONY: help \
	docker_build_image \
	docker_run_bash

docker_build_image:
	docker build . --tag toolchains

docker_run_bash:
	docker run --mount src=".",target=/home/framework,type=bind -it --rm toolchains

help:
	$(info lists of commands:)
	$(info docker_build_image: build docker image based on the dockerfile)
	$(info docker_run_bash: run container in interactive mode)