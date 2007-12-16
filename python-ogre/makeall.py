import subprocess
import environment

default_projects = []
for name,cls in environment.projects.items():
    if cls.active:
        process = subprocess.Popen (["scons", "PROJECTS="+name], shell=True,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print "Processing:", name,
        out,err = process.communicate()
        returncode = process.returncode
        if returncode != 0:
            print "ERROR:",returncode, "  ", out, err
        else:
            print "OK"
            
