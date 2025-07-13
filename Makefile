# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Define custom functions
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
platformpth = $(subst /,$(PATHSEP),$1)

# Set global macros
buildDir := bin
executable := app
target := $(buildDir)/$(executable)
sources := $(call rwildcard,src/,*.cpp)
objects := $(patsubst src/%, $(buildDir)/%, $(patsubst %.cpp, %.o, $(sources)))
depends := $(patsubst %.o, %.d, $(objects))
compileFlags := -std=c++17 -I include
linkFlags = -L lib/$(platform) -l raylib

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := Windows
	CXX ?= g++
	linkFlags += -Wl,--allow-multiple-definition -pthread -lopengl32 -lgdi32 -lwinmm -static -static-libgcc -static-libstdc++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := Linux
		CXX ?= g++
		linkFlags += -l GL -l m -l pthread -l dl -l rt -l X11
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macOS
		CXX ?= clang++
		linkFlags += -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
	endif

	# Set UNIX macros
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif

# Lists phony targets for Makefile
.PHONY: all setup submodules execute clean

# Default target, compiles, executes and cleans
all: $(target) execute clean

# Sets up the project for compiling, generates includes and libs
setup: include lib

# Pull and update the the build submodules
submodules:
	git submodule update --init --recursive --depth 1

# Copy the relevant header files into includes
include: submodules
	$(MKDIR) $(call platformpth, ./include)
	$(call COPY,vendor/raylib/src,./include,raylib.h)
	$(call COPY,vendor/raylib/src,./include,raymath.h)
	$(call COPY,vendor/raylib-cpp/include,./include,*.hpp)

# Build the raylib static library file and copy it into lib
lib: submodules
	cd vendor/raylib/src $(THEN) "$(MAKE)" PLATFORM=PLATFORM_DESKTOP
	$(MKDIR) $(call platformpth, lib/$(platform))
	$(call COPY,vendor/raylib/src,lib/$(platform),libraylib.a)

# Link the program and create the executable
$(target): $(objects)
	$(CXX) $(objects) -o $(target) $(linkFlags)

# Add all rules from dependency files
-include $(depends)

# Compile objects to the build directory
$(buildDir)/%.o: src/%.cpp Makefile
	$(MKDIR) $(call platformpth, $(@D))
	$(CXX) -MMD -MP -c $(compileFlags) $< -o $@ $(CXXFLAGS)

# Run the executable
execute:
	$(target) $(ARGS)

# Clean up all relevant files
clean:
	$(RM) $(call platformpth, $(buildDir)/*)

# The 'release' target builds an optimized, stripped executable for distribution.
# It overrides the default compileFlags and linkFlags for this specific build.
release: CXXFLAGS := $(compileFlags) -O2 -s -fPIC # Add release flags
release: LDFLAGS := $(linkFlags) # Use existing linkFlags (which might include -static)
release: $(target)
	@echo "--- Release build complete: $(target) ---"

# The 'package' target builds the release version and creates a distributable archive.
package: release
	@echo "--- Creating distribution package ---"
	# Ensure the executable directory exists (might have been created by 'release')
	$(MKDIR) $(call platformpth, $(buildDir)) # Renamed from 'bin' in your current Makefile to 'build'

	# IMPORTANT: Create a 'resources' subdirectory next to your executable for assets.
	# You MUST ensure your game loads assets from this 'resources/' path relative to the executable.
	$(MKDIR) $(call platformpth, $(buildDir)/resources)

	# Copy resources from the 'resources' directory to the build directory NEED MANUALLY TO DO
	-robocopy "$(call platformpth,resources)" "$(call platformpth,$(buildDir)/resources)" *.*
	@echo robocopy $(subst /,\,$(CURDIR)/resources) $(subst /,\,$(CURDIR)/$(buildDir)/resources) /E

ifeq ($(OS), Windows_NT)
	# Windows packaging using PowerShell Compress-Archive
	powershell Compress-Archive -Path "$(call platformpth,$(buildDir))\*" -DestinationPath "$(call platformpth,$(buildDir))/../$(executable)-windows.zip" -Force
else ifeq ($(shell uname),Darwin)
	# macOS packaging using tar (creates a .tar.gz from contents of the 'bin' folder)
	tar -czvf $(buildDir)/../$(executable)-macos.tar.gz -C $(buildDir) .
else
	# Linux packaging using tar (creates a .tar.gz from contents of the 'bin' folder)
	tar -czvf $(buildDir)/../$(executable)-linux.tar.gz -C $(buildDir) .
endif
	@echo "--- Package created at $(buildDir)/../$(executable)-$(platform).zip/.tar.gz ---"