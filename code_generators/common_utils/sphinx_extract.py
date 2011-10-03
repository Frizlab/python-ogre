import pyplusplus.decl_wrappers as dw
TABSIZE=4

def getSignature ( dec ):
    """ parse the arguments and return a nice string """
    optional = False
    ret = ""
    param=[]
    try:
        for arg in dec.arguments:
            if arg.default_value:
                ret += ", [" + arg.name + "=" +arg.default_value +"]"
            else:
                ret += ", " + arg.name
            param.append ([arg.name,arg.type.decl_string])
    except:
        print "**ISSUE:", type (dec)
    if ret:
        return ret[2:], param # remove leading comma/space in return string
    else:
        return " ", param # in case there aren't any arguments

def cleanString ( docin, indent = 0 ):
   s = docin.strip()
   if s.startswith ('///') or s.startswith ('/**'):
      s = s[3:].strip()
   elif s.startswith ('/*') or s.startswith ('//'):
      s = s[2:].strip()
   
   if len(s) < 1:  # it might have been a leading comment on a blank line
      return ''
   if s.endswith ( '*/'):
      if s.strip() == '*/':
         return ''
      s = s[:-2].rstrip()
   s = s.rstrip()   ## remove the trailing new line...
   return s

class doc_extractor:
   """
   extracts doxigen styled documentation from source
   or generates from description
   """
   def __init__(self, startswith="", outfile="doc.out"):
      #for caching source
      self.file_name  = None
      self.source = None
      self.startswith = startswith
      self.outfile = open ( outfile, 'w' ) # create the file for writing (truncates existing)
   
   def getDocString ( self ):
      print "CHECKING:", self.declaration, self.declaration.location.line

      maxBackup = 50 # look up to 50 lines back for a comment/docstring
      end = self.declaration.location.line - maxBackup
      if end < 0: end = 0
      start = self.declaration.location.line - 1 # adjust for zero based and 1 based numbering scheme
      if start < 0: start = 0
      
      ret = []
      
      for lcount in xrange(start, end , -1):
         line = self.source[lcount].strip()
         if line[:2] == '//': # really should care about '///' but we'll use '//'
         	# roll backwards adding all comment lines starting with // -- note that if there is a blank
         	# then we will not get comments above it..
         	# we are assuming there aren't any format commands '@' in simple comments
            while self.source[lcount].strip()[:2] == '//' and lcount >0:
               ret.insert ( 0, cleanString(self.source[lcount]) ) # add doc string to return attay
               lcount -= 1
            break
         elif line[-2:]=='*/':
            if line[:2] == '/*':  # it's a one line comment
               ret.insert ( 0, cleanString (line) )
               break
            else:
               # OK it's a more complete comment and will need processing for '@' commands	
               while not self.source[lcount].strip()[:2] == '/*' and lcount >0:
                  ret.insert ( 0, self.source[lcount] ) # add doc string to return attay
                  lcount -= 1
               ret.insert ( 0, self.source[lcount] ) # add doc string to return attay
               break
               
      # remove simple blank lines         
      if len(ret)>0:
         if ret[0].strip() == '//' or ret[0].strip()=='/*':
            del ret[0]
      if len(ret)>0:
         if ret[-1].strip() == '//' or ret[-1].strip()=='*/':
            del ret[-1]
            
	  # now replace tabs with spaces            
      s='                     '
      maxspace = 0
      for x in range ( len (ret)):
         ret[x] = ret[x].replace('\t', s[:TABSIZE])
      return ret


   
   def cleanEntireDoc ( self, docin ):
      """
      General cleaup routine - replace any 'bad' characters, manage line lengths
      set leading spaces for appropiate indent etc
      """

   
   def getHeaderDoc ( self, declaration ):
      """
      Extract documentation from the header file and return a 'useful
      dictionary containing everything we were able to find.
      
      Each dict entry is a list of extracted text
      """
      if not declaration.location:
         return {}
      
      if self.file_name != declaration.location.file_name:    # don't want to open the file multiple times.
         self.file_name = declaration.location.file_name
         self.source = open(declaration.location.file_name).readlines()
         
         # lets not worry about 'small' files as theres probalby an issue
         if len ( self.source ) < 5 :
            return {}
      
      ## note the gccxml uses a 1 based line scheme whereas we are using python arrays - 0 based
      ## so we need to subtract 1 from the line number.
      ##
      ## then we need to subtract another 1 to put us on the line above the function/class etc defination
      ##
      ## AND to make it worse, Ogre coding style often has the opening brace on the line AFTER the defination name
      ## and gccxml gives us the line number with the brace - so we need to look back even further
      ##
      ## lets look for a single comment '///'  We'll try upto 2 lines above
      d = self.getDocString ()
      return d
      


   
   def __call__(self, declaration):
      self.declaration = declaration
      print self.getHeaderDoc ( declaration )
#      print getSignature ( declaration )

