#!/usr/bin/env python
# 
# Copyright (c) 2009, Roboterclub Aachen e.V.
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

import os
import string
import time
import re
from SCons.Script import *

generationBlockString = """/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------"""

# -----------------------------------------------------------------------------
def template_action(target, source, env):
	if not env.has_key('substitutions'):
		raise SCons.Errors.UserError, "Use 'Template(..., substitutions = ...)'"
	
	source = source[0].abspath
	target = target[0].abspath
	
	output = string.Template(open(source, 'r').read()).safe_substitute(env['substitutions'])
	
	open(target, 'w').write(output)
	return 0

# -----------------------------------------------------------------------------
def jinja2_template_action(target, source, env):
	if not env.has_key('substitutions'):
		raise SCons.Errors.UserError, "Use 'Jinja2Template(..., substitutions = ...)'"
	
	try:
		import jinja2
	except ImportError:
		print "To use this functionality you need to install the jinja2 template engine"
		Exit(1)
	
	globals = {
		'time': time.strftime("%d %b %Y, %H:%M:%S", time.localtime()),
		'match': re.match,
		'generation_block': generationBlockString,
	}
	
	def filter_wordwrap(value, width=79):
		return '\n\n'.join([textwrap.fill(str, width) for str in value.split('\n\n')])

	def filter_indent(value, level=0):
		return ('\n' + '\t' * level).join(value.split('\n'))
		
	path, filename = os.path.split(source[0].path)
	loader = jinja2.Environment(loader = jinja2.FileSystemLoader(path))
	loader.filters['xpcc.wordwrap'] = filter_wordwrap
	loader.filters['xpcc.indent'] = filter_indent
	# Jinja2 Line Statements
	loader.line_statement_prefix = '%%'
	loader.line_comment_prefix = '%#'

	template = loader.get_template(filename, globals=globals)
	
	output = template.render(env['substitutions'])
	open(target[0].path, 'w').write(output)

def template_emitter(target, source, env):
	Depends(target, SCons.Node.Python.Value(env['substitutions']))
	return target, source

def template_string(target, source, env):
	return "Template: '%s' to '%s'" % (str(source[0]), str(target[0]))

# -----------------------------------------------------------------------------
def generate(env, **kw):
	env.Append(
		BUILDERS = {
		'Template': env.Builder(
			action = env.Action(template_action, template_string),
			emitter = template_emitter,
			src_suffix = '.in',
			single_source = True
		),
		
		'Jinja2Template':  env.Builder(
			action = env.Action(jinja2_template_action, template_string),
			emitter = template_emitter,
			src_suffix = '.in',
			single_source = True
		),
	})

def exists(env):
	return True
