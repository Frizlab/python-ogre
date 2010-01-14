from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
### python BuildCython.py build_ext --inplace
setup(
    cmdclass = {'build_ext': build_ext},
    language='c++',
    extra_compile_args = ' O2',
    include_dirs=["c:\development\Ogre_SVN\ogremain\include"],
    ext_modules = [Extension("Helper_CalcNorms", ["Helper_CalcNorms.pyx"])]
)