"""Commandline script to generate a Chm for an epydoc project.


@todo: currently B{all} files encountered in the directory are included
in the Chm. No test is available if the directory actually contains
epydoc output. 
"""

import os, sys

import chm_compiler
from chm_project import Project 
#********************************************************************************
#
#********************************************************************************
USAGE = (
	'Usage: python gen_epydoc.py [options] <directory>'
	'\n'
	'\n'
	'Creates Chm project files from a directory containing epydoc generated documentation'
	)

DEFAULT_NAME = 'quick-and-dirty-dump'

#********************************************************************************
#
#********************************************************************************
def main(
		directory, 
		compiler=None,
		do_compile=False, 
		name=DEFAULT_NAME, 
		output_directory=None, 
		):
	"""Compiles a Chm from a directory containing epydoc generated documentation
	@param compiler: path to the Chm compiler executable or C{None}
	@param directory: directory containing the tree of files
	@param name: name of the project
	@param output_directory: directory where the Chm should be generated
	@return: C{None}
	@rtype: None	
	"""
	
	
	project_file = os.path.join(directory, name)
	if output_directory:
		compiled_file = os.path.join(output_directory, name)	
	else:
		compiled_file = os.path.join(directory, name)	
	
	
	# compile chm (if possible)
	project = chm_compiler.write_project(
		directory,
		project_file,
		compiler=compiler,
		compiled_file=compiled_file,
		input_walker='epydoc',
		window_title=name,
		)
	
	if do_compile:
		chm_compiler.compile_chm(project)
	


#********************************************************************************
#
#********************************************************************************
if __name__ == '__main__':
	
	import optparse
		
	o = optparse.OptionParser(usage=USAGE)
	o.add_option(
	    "-n",
		"--name",
		dest="name",
		help="Name of the Chm"
		)
	o.add_option(
		"--compile",
		dest="compile",
		default="True",
		help="Optional flag. If specified, compiles a Chm"
		)
	o.add_option(
		"-o",
		"--ouput_dir",
		dest="output_dir",
		help="Output directory of the Chm"
		)
	o.add_option(
	    "-c",
		"--compiler",
		dest="compiler",
		help="Path of the Chm compiler executable"
		)
		
	
	# helper for optparse...
	def optparse_options_to_dict(options):
		options = options.__dict__	
		for name, value in options.items():
			if value == None:
				del options[name]
		return options
	
	print "checking options"
	options, names = o.parse_args()
	print "parsed"
	options = 	optparse_options_to_dict(options)
	if names:
		directory = names[0]
		sys.exit(
			main(
				directory, 
				name=options.get('name', DEFAULT_NAME), 
				do_compile=options.get('compile', False),
				output_directory=options.get('output_dir', directory),
				compiler = options.get('compiler', None),
				)
			)
			
	o.print_help()
	

		




