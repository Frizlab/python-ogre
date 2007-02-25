##
## Convert Ogre CPP code to Python-Ogre
##
import sys, re, os

ReplaceString = ""
InlineConverters = [
    (r'//', '##'),
    (r'->', '.'),
    (r'::', '.'),
    ('true', 'True'),
    ('false', 'False'),
    (';', ' '), 
    ('const', ''),
    (r'[0-9]f', 'special1'), 
    (r'^[\W]*{[\W]*$\n',''),
    (r'^[\W]*}[\W]*$\n',''),
    (r'void \W*(\w+)\(','def $1('),
    (r'^[\W]*if.*', 'special2'),
    (r'^[\W]*else.*', 'special2')   ## ,
##    (r'^/\*.*\*/', 'special3')
    ]
    
def replaceit ( matchobj ) :
    global ReplaceString
    s = matchobj.group(0)
    if s[-1:]=='f':
        return s[:-1]
    if ReplaceString == 'special2':
        return s + ':'
    if ReplaceString == 'special3':
        print "***", matchobj.group(0)
    return ReplaceString

def remove_comments ( buf ):
    return
    singlestring = ""
    newstring=""
    for line in buf:
        singlestring += line
       
    ##cp = re.compile( r'/\*.+\*/', re.MULTILINE )
    cp = re.compile( r'/\*.*\*/', re.MULTILINE|re.DOTALL )
    ##cp = re.compile( r'\*/', re.MULTILINE )
    ##res = cp.search(singlestring)
    res = cp.findall(singlestring)
    currpos = 0
    if res:
        newstring += singlestring[0:res.start(res.group(0) )]   # copy the first bit
        for g in res.groups():
            newstring += res.string[res.start(g):res.end(g)]
    else:   ## didn't match
        return singlestring
               
        
def convert_file( infile ):
    global ReplaceString
    textout = []
    file = open ( infile )
    for line in file:
        newline = line
        for match, replace in InlineConverters:
            ReplaceString = replace
            newline = re.sub ( '('+match+')', replaceit, newline )
            #print "**", line, newline
            ### m = re.search('('+match+')', line )
        textout.append( newline)
            
    return textout

def write_file ( buf, outfile):
    for line in buf:
        outfile.write(line)
    outfile.close()
    
if __name__ == '__main__':
    if len (sys.argv) < 2 :
        print "Need to specifiy a file to convert"
        sys.exit(-1)
    #get us to the correct directory
    path = os.path.split(os.path.abspath(sys.argv[0]))[0]
    infile = sys.argv[1]
    filename = os.path.basename ( infile )
    name, ext = filename.split('.')
    outfile = open (name + ".py", 'w')
    TextBuffer = convert_file( infile )
    newbuffer = remove_comments ( TextBuffer )
    write_file ( TextBuffer, outfile)
    print 'Converted File'
    