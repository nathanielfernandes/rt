NAME=rt
BUILD=./build
VENDOR=$(BUILD)/vendor
INCLUDE=$(VENDOR)/include
LIB=$(VENDOR)/lib

SDL=./submodules/SDL

# Build SDL2
sdl2:
	@mkdir -p $(SDL)/build
	@cd $(SDL)/build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release --parallel

	# Copy SDL2 libraries to vendor
	@cp $(SDL)/build/libSDL2-2.0.dylib $(LIB)/libSDL2.dylib

	# Copy SDL2 headers to vendor
	@mkdir -p $(INCLUDE)/SDL2
	@cp $(SDL)/include/*.h $(INCLUDE)/SDL2

deps: sdl2


# Setup build for rt
setup: 
	@mkdir -p $(BUILD)
	@mkdir -p $(VENDOR)
	@mkdir -p $(INCLUDE)
	@mkdir -p $(LIB)

	@$(MAKE) deps

	@cd $(BUILD) && cmake ..

clean: 
	rm -rf $(BUILD)

clean-deps:
	rm -rf $(SDL)/build



.PHONY: build
build: 
	@$(MAKE) -C $(BUILD)

run: build
	@$(BUILD)/$(NAME)