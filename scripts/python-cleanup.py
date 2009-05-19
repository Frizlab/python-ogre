
import os
import subprocess
import py_compile

p = subprocess.Popen(["find", "-name", "*.py"], stdout=subprocess.PIPE,
                     shell=False)
p.wait()

for f in p.stdout.readlines():
  f = f.strip()
  try:
    py_compile.compile(f, doraise=True)
  except:
    print "Removing %s as it didn't compile" % f
    os.unlink(f)
