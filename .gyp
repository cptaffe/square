
# Copyright (c) 2015 Connor Taffe. All rights reserved.

{
	'targets': [
		{	# window library
			'target_name': 'libwin',
			'type': 'static_library',
			'sources': [
				'win/win.cc'
			],
			'defines': [
				'_REENTRANT',
			],
			'include_dirs': [
				'win',
				# SDL include on Fedora
				'/usr/include/SDL2'
			],
			'cflags': [
				'--std=c++11'
			],
		},
		{	# main executable
			'target_name': 'sdl',
			'type': 'executable',
			'dependencies': [
				'libwin',
			],
			'sources': [
				'main.cc',
			],
			'include_dirs': [
				'win',
				# SDL include on Fedora
				'/usr/include/SDL2'
			],
			'libraries': [
				# SDL flags on Fedora
				'<!(pkg-config --libs-only-l sdl2)',
				'-lGL',
			],
		}
	]
}
