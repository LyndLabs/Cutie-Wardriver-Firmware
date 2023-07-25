PORT ?= /dev/ttyUSB0
IMAGE_NAME = wardriver
CONTAINER_NAME := $(IMAGE_NAME)-$(shell date +%s)

submodules:
	git submodule init
	git submodule update

build:
	docker build . --file Dockerfile --tag wardriver

flash8266: check_port build
	docker create --name $(CONTAINER_NAME) --device=$(PORT) -t wardriver:latest
	docker start $(CONTAINER_NAME)
	docker exec $(CONTAINER_NAME) bash -c \
		'./arduino-cli upload -b esp8266:esp8266:nodemcuv2 --port $(PORT) Wardriver/ && sleep 2'
	docker rm --force $(CONTAINER_NAME)

flash32: check_port build
	docker create --name $(CONTAINER_NAME) --device=$(PORT) -t wardriver:latest
	docker start $(CONTAINER_NAME)
	docker exec $(CONTAINER_NAME) bash -c \
		'./arduino-cli upload -b esp32:esp32:esp32 --port $(PORT) Wardriver/ && sleep 2'
	docker rm --force $(CONTAINER_NAME)

check_port:
	@ls $(PORT) || { echo "Device not found at $(PORT). Run with 'make flash PORT=<port>'" && exit 1; }
	