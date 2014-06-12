#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name='Arduino',
      version='1.0',
      description='Python Arduino',
      author='Wyliodrin',
      author_email='office@wyliodrin.com',
      url='http://www.github.com/Wyliodrin/libwyliodrin',
      ext_modules = [Extension('_arduino', ['arduino.i'], 
                             swig_opts=['-modern', '-I../../arduino'],include_dirs=['../../arduino'], libraries=['wiringarduino'] )],
     )

