#!/usr/bin/env python

from distutils.core import setup, Extension


wyliodrin_module = Extension('_wyliodrin',
                           sources=['wyliodrin_wrap.cxx'],
                           libraries=['wyliodrin'],
                           )

setup (name = 'wyliodrin',
       version = '0.1',
       author      = "Wyliodrin",
       description = """Python Wyliodrin library""",
       ext_modules = [wyliodrin_module],
       py_modules = ["wyliodrin"],
       )


