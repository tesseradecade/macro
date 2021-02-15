from distutils.core import setup
from distutils.extension import Extension

setup(name="macro",
      ext_modules=[
            Extension(
                  "cmacro",
                  sources=["macro/py_macro.c"]
            )
      ])
