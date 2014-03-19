#!/usr/bin/env python
# 
# Copyright (c) 2012, Roboterclub Aachen e.V.
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Roboterclub Aachen e.V. nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# -----------------------------------------------------------------------------
# 
# DESCRIPTION
# 
# This is a tool compile programs for the AVR32 microcontrollers from Atmel.
# The 'avr32-g++' is used as compiler.
# 
# EXAMPLE
# 
# A simple SConstruct file to compile a file named 'main.c' for an ATmega644
# can look as follows:
# 
#    env = Environment(tools = ['avr32'],
#             AVR32_DEVICE = 'uc3b0256',
#             AVR32_CLOCK = 16000000)
#    
#    program = env.Program('main.c')
#    
#    env.Hex(program)
#    env.Listing(program)
#
# BUILDERS
# 
# The following builders are available:
# 
# Object()      .c   -> .o
# Object()      .cpp -> .o
# Object()      .S   -> .o
# Library()     .o   -> .a
# Program()     .o   -> .elf
# Hex()         .elf -> .hex
# Eeprom()      .elf -> .eep
# Listing()     .elf -> .lss
#
# The 'Program' builder will automatically call the 'Object' builder so
# it's no need to use it explicitly. Just call 'Program' with all your
# source files.
#
# TODO
# 
# - Make the optimization level adjustable

import os
from SCons.Script import *
import SCons.Subst
import tempfile
import os

# -----------------------------------------------------------------------------
def generate(env, **kw):
	env.Append(ENV = {'PATH' : os.environ['PATH']})
	env.Tool('gcc')
	env.Tool('g++')
	env.Tool('gnulink')
	env.Tool('ar')
	env.Tool('as')
	
	env['PROGSUFFIX'] = ".elf"
	
	# used programs
	env['CC']      = "avr32-gcc"
	env['CXX']     = "avr32-g++"
	env['AS']      = "avr32-as"
	env['OBJCOPY'] = "avr32-objcopy"
	env['OBJDUMP'] = "avr32-objdump"
	env['AR']      = "avr32-ar"
	env['NM']      = "avr32-nm"
	env['RANLIB']  = "avr32-ranlib"
	env['SIZE']    = "avr32-size --format=sysv -x"
	
	# build messages
	if ARGUMENTS.get('verbose') != '1':
		env['CCCOMSTR']     = "Compiling C: $TARGET"
		env['CXXCOMSTR']    = "Compiling C++: $TARGET"
		env['ASCOMSTR']     = "Assembling: $TARGET"
		env['ASPPCOMSTR']   = "Assembling: $TARGET"
		env['LINKCOMSTR']   = "Linking: $TARGET"
		env['RANLIBCOMSTR'] = "Indexing: $TARGET"
		env['ARCOMSTR']     = "Create Library: $TARGET"
		
		env['SIZECOMSTR']    = "Size after:"
		env['SYMBOLSCOMSTR'] = "Show symbols for '$SOURCE':"
		env['HEXCOMSTR']     = "Creating load file for Flash: $TARGET"
		env['EEPROMCOMSTR']  = "Creating load file for EEPROM: $TARGET"
		env['LSSCOMSTR']     = "Creating Extended Listing: $TARGET"
	
	# C flags
	env['CFLAGS'] = [
		"-std=gnu99",
		"-Wstrict-prototypes",
		"-Wredundant-decls",
		"-Wnested-externs",
#		"-fpack-struct",		# WARNING: incompatible with Atmel's interrupt handler
	]
	
	# flags for C and C++
	env['CCFLAGS'] = [
		"-march=ucr1",
		"-mpart=$AVR32_DEVICE", 
		"-Os",
#		"-gdwarf-2",
		"-g3",
		"-MD",
		"-MP",
		"-MQ",
		"-funsigned-char",
		"-funsigned-bitfields",
		"-fshort-enums",
		"-ffunction-sections",
		"-fdata-sections",
		"-fno-split-wide-types",
		"-fno-move-loop-invariants",
		"-fno-tree-loop-optimize",
		"-finline-limit=10000",
#		"-fverbose-asm",	# TODO check this
#		"-masm-addr-pseudos",
		"-mrelax",
		"-pipe",
		"-Wall",
		"-Wextra",
		"-Wundef",
#		"-Winline",
		"-Wformat",
		"-Winit-self",
#		"-Wcast-qual",		# TODO enable this
		"-Wimplicit",
#		"-Wshadow",			# TODO enable this
		"-Wpointer-arith",
		"-Wunused",
		"-Wa,-adhlns=${TARGET.base}.lst",
		"-DBASENAME=${SOURCE.file}",
		"-mno-cond-exec-before-reload", 	# fixes compiler error? TODO check if bugfix exists
	]
	
	# C++ flags
	env['CXXFLAGS'] = [
		"-std=gnu++0x",
		"-fno-exceptions", 
		"-fno-rtti",
		"-fno-threadsafe-statics",
		"-fuse-cxa-atexit",
		"-nostdlib",
		"-Woverloaded-virtual",
#		"-Wnon-virtual-dtor",
#		"-Weffc++",
#		"-save-temps",		# save preprocessed files
	]
	
	# Assembler flags
	env['ASFLAGS'] = [
		"-march=ucr1",
		"-mpart=$AVR32_DEVICE",
		"-MD",
		"-MP",
		"-MQ",
		"-gdwarf-2",
		"-mrelax",
		"-D__ASSEMBLY__",
		"-Wa,-adhlns=${TARGET.base}.lst",
		"-xassembler-with-cpp",
	]
	
	# Determine linker script from selected processor. 
	# This could be replaced by a dictionary as in arm_devices.py
	
	# AVR32_DEVICE is e.g. 'uc3b0256'
	# get number: 064, 164, 1256, 0256, 1128, 0512, 1512 or 0128
	# get device: uc3b
	avr32_number = env['AVR32_DEVICE'][4:]
	avr32_device = 'at32' + env['AVR32_DEVICE'][0:4]
	
	env['LINKFILE'] = ["%s/ext/asf/avr32/utils/linker_scripts/%s/%s/gcc/link_%s.lds" % (env['XPCC_ROOTPATH'], avr32_device, avr32_number, env['AVR32_DEVICE'])]
	
	# Link flags
	env['LINKFLAGS'] = [
		"-mpart=$AVR32_DEVICE", 
		"-T${LINKFILE}",
		"-Wl,--relax",
		"-Wl,--gc-sections",
		"-nostartfiles",
		'-Wl,-e,_trampoline',				# place a jump to 0x80002000 at 0x80000000. Needed for DFU bootloader. 
		"-Wl,-Map=${TARGET.base}.map,--cref", 
#		"-Wl,-u,vfprintf -lprintf_flt"		# enable float support for vfprinft
	]

	env['LINKCOM'] = "$LINK -o $TARGET $LINKFLAGS -lm -lc -lm $SOURCES -lm $_LIBDIRFLAGS $_LIBFLAGS -lm"

	clock = str(env['AVR32_CLOCK']).lower()
	if not clock.endswith('ul'):
		clock += 'ul'
	env.Append(CPPDEFINES = {'F_CPU' : clock})
	
	builder_hex = Builder(
		action = Action("$OBJCOPY -O ihex -R .eeprom $SOURCE $TARGET",
						cmdstr = "$HEXCOMSTR"), 
		suffix = ".hex", 
		src_suffix = ".elf")

	builder_eeprom = Builder(
		action = Action("$OBJCOPY -j .eeprom --set-section-flags=.eeprom=\"alloc,load\" --change-section-lma .eeprom=0 --no-change-warnings --change-section-lma .eeprom=0 -O ihex $SOURCE $TARGET",
						cmdstr = "$EEPROMCOMSTR"), 
		suffix = ".eep",
		src_suffix = ".elf")

	builder_listing = Builder(
		action = Action("$OBJDUMP -h -S $SOURCE > $TARGET",
						cmdstr = "$LSSCOMSTR"), 
		suffix = ".lss", 
		src_suffix = ".elf")
	
	env.Append(BUILDERS = {
		'Hex'    : builder_hex,
		'Eeprom' : builder_eeprom,
		'Listing': builder_listing,
	})

	# changes env['ARCOM'] and env['LINKCOM']
	if str(Platform()) == "win32":
		env.Tool('gcc_windows')

# -----------------------------------------------------------------------------
def exists(env):
	return env.Detect('avr32-gcc')
