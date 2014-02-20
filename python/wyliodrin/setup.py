#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name='Lib Wyliodrin',
      version='1.0',
      description='Python Lib Wyliodrin',
      author='Wyliodrin',
      author_email='office@wyliodrin.com',
      url='http://www.github.com/Wyliodrin/libwyliodrin',
      ext_modules = [Extension('_libwyliodrin', ['libwyliodrin.i'], 
                             swig_opts=['-modern', '-I .'], libraries=['wyliodrin'] )],
      py_modules = ['wyliodrin', 'libwyliodrin'],
     )

