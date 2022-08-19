NAME=rt
BUILD=./build
VENDOR=$(BUILD)/vendor
INCLUDE=$(VENDOR)/include
LIB=$(VENDOR)/lib

# SDL=./submodules/SDL
GLEW=./submodules/glew
GLM=./submodules/glm
GLFW=./submodules/glfw
TOBJ=./submodules/tinyobjloader
GLSLSI=./submodules/GLSL-Shader-Includes
RADEONRAYS=./submodules/RadeonRays


# Build SDL2
# sdl2:
# 	@mkdir -p $(SDL)/build
# 	@cd $(SDL)/build && cmake .. -DCMAKE_BUILD_TYPE=Release && cmake --build . --config Release --parallel

# 	# Copy SDL2 libraries to vendor
# 	@cp $(SDL)/build/libSDL2-2.0.dylib $(LIB)/libSDL2.dylib

# 	# Copy SDL2 headers to vendor
# 	@mkdir -p $(INCLUDE)/SDL2
# 	@cp $(SDL)/include/*.h $(INCLUDE)/SDL2

# setup glfw
glfw:
	@cp -R $(GLFW)/include/GLFW $(INCLUDE)/GLFW


# Build glew
glew:
	@cd $(GLEW)/build && cmake ..
	@$(MAKE) -C $(GLEW)/build

	# Copy glew libraries to vendor
	@cp $(GLEW)/build/lib/libglew.a $(LIB)/libglew.a

	# Copy glew headers to vendor
	@mkdir -p $(INCLUDE)/GL
	@cp $(GLEW)/include/GL/*.h $(INCLUDE)/GL


# setup glm
glm:
	@cp -R $(GLM)/glm $(INCLUDE)/glm

# setup tiny_obj_loader
tiny_obj_loader:
	@cp $(TOBJ)/tiny_obj_loader.h $(INCLUDE)/tiny_obj_loader.h

# setup GLSL-Shader-Includes
glslsi:
	@cp $(GLSLSI)/Shadinclude.hpp $(INCLUDE)/Shadinclude.hpp

# setup RadeonRays
radeonrays:
	@mkdir -p $(INCLUDE)/RadeonRays
	@cp $(RADEONRAYS)/*.hpp $(INCLUDE)/RadeonRays

deps: glfw glew glm tiny_obj_loader glslsi radeonrays

# Setup build for rt
setup: 
	@mkdir -p $(BUILD)
	@mkdir -p $(VENDOR)
	@mkdir -p $(INCLUDE)
	@mkdir -p $(LIB)

	@$(MAKE) deps

	@cd $(BUILD) && cmake -DCMAKE_BUILD_TYPE=Release ..

clean: 
	rm -rf $(BUILD)

# rm -rf $(SDL)/build
clean-deps:
	rm -rf $(GLEW)/build


.PHONY: build
build: 
	@$(MAKE) -C $(BUILD)

run: build
	@$(BUILD)/$(NAME)