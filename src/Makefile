SRC_DIR = $(shell pwd)
SOURCES := $(shell find . -name '*.cpp' -o -name '*.hpp')

BUILD_DIR = $(SRC_DIR)/../BUILD/
GEN_MAKEFILE = ../Makefile
PROJECT = move-on-helium-sensors

.PHONY: build flash clean

build: $(BUILD_DIR)/$(PROJECT).hex Makefile

flash: $(BUILD_DIR)/$(PROJECT).hex
	st-flash --format ihex write $<

clean:
	-rm ../Makefile
	-rm -r $(BUILD_DIR)

$(GEN_MAKEFILE):
	cd .. && mbed export -i gcc_arm

$(BUILD_DIR)/$(PROJECT).hex: $(GEN_MAKEFILE) $(SOURCES)
	$(MAKE) -C .. -j 8