from distutils.core import setup, Extension, DEBUG

sfc_module = Extension('pass_array', sources = ['Source.cpp'])

setup(name = 'pass_array', version = '1.0',
	description = 'Python Package with pass_array C++ extension',
	ext_modules = [sfc_module]
)
