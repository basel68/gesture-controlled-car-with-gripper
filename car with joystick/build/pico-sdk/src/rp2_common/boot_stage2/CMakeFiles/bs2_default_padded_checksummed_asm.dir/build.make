# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CMake\bin\cmake.exe

# The command to remove a file.
RM = D:\CMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\gesture-controlled-car-with-gripper\car with joystick"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\gesture-controlled-car-with-gripper\car with joystick\build"

# Utility rule file for bs2_default_padded_checksummed_asm.

# Include any custom commands dependencies for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/compiler_depend.make

# Include the progress variables for this target.
include pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/progress.make

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm: pico-sdk/src/rp2_common/boot_stage2/bs2_default_padded_checksummed.S

pico-sdk/src/rp2_common/boot_stage2/bs2_default_padded_checksummed.S: pico-sdk/src/rp2_common/boot_stage2/bs2_default.bin
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir="D:\gesture-controlled-car-with-gripper\car with joystick\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Generating bs2_default_padded_checksummed.S"
	cd /d "D:\gesture-controlled-car-with-gripper\car with joystick\build\pico-sdk\src\rp2_common\boot_stage2" && C:\ProgramData\Anaconda3\python.exe D:/VSARM/sdk/pico/pico-sdk/src/rp2_common/boot_stage2/pad_checksum -s 0xffffffff "D:/gesture-controlled-car-with-gripper/car with joystick/build/pico-sdk/src/rp2_common/boot_stage2/bs2_default.bin" "D:/gesture-controlled-car-with-gripper/car with joystick/build/pico-sdk/src/rp2_common/boot_stage2/bs2_default_padded_checksummed.S"

pico-sdk/src/rp2_common/boot_stage2/bs2_default.bin: pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir="D:\gesture-controlled-car-with-gripper\car with joystick\build\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Generating bs2_default.bin"
	cd /d "D:\gesture-controlled-car-with-gripper\car with joystick\build\pico-sdk\src\rp2_common\boot_stage2" && "D:\program files2\GNU TOOL CHAIN\12.3 rel1\bin\arm-none-eabi-objcopy.exe" -Obinary "D:/gesture-controlled-car-with-gripper/car with joystick/build/pico-sdk/src/rp2_common/boot_stage2/bs2_default.elf" "D:/gesture-controlled-car-with-gripper/car with joystick/build/pico-sdk/src/rp2_common/boot_stage2/bs2_default.bin"

bs2_default_padded_checksummed_asm: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm
bs2_default_padded_checksummed_asm: pico-sdk/src/rp2_common/boot_stage2/bs2_default.bin
bs2_default_padded_checksummed_asm: pico-sdk/src/rp2_common/boot_stage2/bs2_default_padded_checksummed.S
bs2_default_padded_checksummed_asm: pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/build.make
.PHONY : bs2_default_padded_checksummed_asm

# Rule to build all files generated by this target.
pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/build: bs2_default_padded_checksummed_asm
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/build

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/clean:
	cd /d "D:\gesture-controlled-car-with-gripper\car with joystick\build\pico-sdk\src\rp2_common\boot_stage2" && $(CMAKE_COMMAND) -P CMakeFiles\bs2_default_padded_checksummed_asm.dir\cmake_clean.cmake
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/clean

pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\gesture-controlled-car-with-gripper\car with joystick" D:\VSARM\sdk\pico\pico-sdk\src\rp2_common\boot_stage2 "D:\gesture-controlled-car-with-gripper\car with joystick\build" "D:\gesture-controlled-car-with-gripper\car with joystick\build\pico-sdk\src\rp2_common\boot_stage2" "D:\gesture-controlled-car-with-gripper\car with joystick\build\pico-sdk\src\rp2_common\boot_stage2\CMakeFiles\bs2_default_padded_checksummed_asm.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : pico-sdk/src/rp2_common/boot_stage2/CMakeFiles/bs2_default_padded_checksummed_asm.dir/depend

