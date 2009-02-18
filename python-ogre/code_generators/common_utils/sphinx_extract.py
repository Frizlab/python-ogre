import pyplusplus.decl_wrappers  as dw
TABSIZE=3

def getSignature ( dec ):
    """ parse the arguments and return a nice string """
    optional = False
    ret = ""
    param=[]
    for arg in dec.arguments:
        if arg.default_value:
            ret += ", [" + arg.name + "=" +arg.default_value +"]"
        else:
            ret += ", " + arg.name  
        param.append ([arg.name,arg.type.decl_string])          
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
      maxBackup = 5 # look up to 5 lines back for a comment/docstring
      end = self.declaration.location.line - maxBackup
      if end < 0: end = 0
      start = self.declaration.location.line+1
      if start < 0: start = 0
      
      ret = []
#       print start, end, self.declaration, self.declaration.location.line
      for lcount in xrange(start, end , -1):
         line = self.source[lcount].strip()
#          print "Checking:", line
         if line[:2] == '//': # really should care about '///' but we'll use '//'
            while self.source[lcount].strip()[:2] == '//' and lcount >0:
               ret.insert ( 0, self.source[lcount] ) # add doc string to return attay
               lcount -= 1
            break  
         elif line[-2:]=='*/':  
#             print "line is end of comment"
            if line[:2] == '/*':  # it's a one line comment
#                print "one line comment"
               ret.insert ( 0, self.source[lcount] )
               break
            else:               
               while not self.source[lcount].strip()[:2] == '/*' and lcount >0:
#                   print "Inserting", self.source[lcount]
                  ret.insert ( 0, self.source[lcount] ) # add doc string to return attay
                  lcount -= 1
               ret.insert ( 0, self.source[lcount] ) # add doc string to return attay
               break
      if len(ret)>0:            
         if ret[0].strip() == '//' or ret[0].strip()=='/*':
            del ret[0]
      if len(ret)>0:            
         if ret[-1].strip() == '//' or ret[-1].strip()=='*/':
            del ret[-1]
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
      print "DOC:", d  
                  
      
      
      
   def __call__(self, declaration):
      self.declaration = declaration
      self.getHeaderDoc ( declaration )       
      
# # # #       for lcount in xrange(declaration.location.line-1, -1, -1):
# # # #       line = self.source[lcount]
# # # #       #print "Checking ", lcount, line
# # # #       if not find_block_end:
# # # #         try:
# # # #             if line.rstrip()[-2:] == "*/":
# # # #                 find_block_end = True
# # # #         except:
# # # #             pass
# # # #       if  find_block_end:
# # # #         try:
# # # #                if line.lstrip()[:2] == "/*":
# # # #                    find_block_end = False
# # # #                  except:
# # # #                      pass
# # # #              final_str = clear_str(line)
# # # #              if not find_block_end and code(line):
# # # #                  ## note that ogre code tends to have the opening function/class bracket on the 'next' line 
# # # #                  ## from the 'name' -so we may need to backup one more line to fine doc string..
# # # #                  failed +=1
# # # #                  final_str = ""  ## make sure we don't add the extra code line
# # # #                  if failed > 2:
# # # #                      break
# # # #              if final_str:
# # # #                  doc_lines.insert(0, clear_str(final_str))
# # # #       except:
# # # #          if not basedoc:
# # # #              return ""
# # # #       ret ="" 
# #         
# #         if doc_lines:
# #             doc_lines = removeBlankLines ( doc_lines )
# #             doc_lines = removeLeadingSpace ( doc_lines )
# #             doc_lines = wrapLines ( doc_lines )
# # #             print "Extracted Doc String for:",  declaration, "[", len(doc_lines),"]"
# #             ## we need to cope with strings longer than 2048 for MSVC
# #             ret =  "\\\n".join(doc_lines) 
# #             if len ( ret ) > 1700:  ## just to be safe and adjust for line end chars etc..
# #                 ret = ""
# #                 length = 1
# #                 for line in doc_lines:
# #                     ret = ret + line
# #                     length += len ( line )
# #                     if length < 1500 :  # again lets be conservative
# #                         ret = ret + "\\\n"
# #                     else :
# #                         ret = ret + '"' + "\n" + '"'    # we close the original 'quote', move to a new line and open a new quote
# #                         length = 1
# #         ## Py++ doesn't like non ascii characters in the doc string
# #         ## class_declaration.py - _generate_constructor - return ( ''.join( result ), used_init )
# #         newret = ""
# # #         print "DOC:  GOT SOMETHING", ret
# #         for c in ret:
# #             if ord(c) >127:
# #                 c = ' '
# #             newret = newret + c
# #         if len(basedoc) >1 or len(newret)>1:
# # # #             self.outfile.write("***============================*****\n" + declaration.decl_string + "\n")
# # # #             self.outfile.write( str(len(basedoc)) + "  " + str(len(newret)) + "\n" )
# # # #             self.outfile.write( newret )      
# #             retval ='"' + basedoc + newret.lstrip() + '"' 
# # #             print "RETURNING:", retval
# #             return retval             
# #         else: 
# #             return ""
# #             
# #             
# #                 
# #     def __call__(self, declaration):
# #         print "declatarion", declaration.decl_string 
# #         print "name:", declaration.name
# #         print "demangled:", declaration.demangled
# #         
# #         print "type:", type(declaration) #.type
# #         
# # 
# #         
# #         
# #         try:    # some types such as the base namespace don't have 'location'
# #             print "location",declaration.location.file_name 
# # #                 self.source = open(declaration.location.file_name).readlines()
# #         except:
# #             print "DOC without location:", declaration
# # #             return ""
# #             
# #         t = type(declaration)
# #         print "Type:", t
# #         
# #         
# #         s=''
# #         defClass=".. class:: %s\n   :members:\n"
# #         defConstructor=".. class:: %s (%s)\n   :members:\n"
# #         defFunction = ".. function %s"
# #         
# #         if t == type (dw.class_wrapper.class_t()):
# #             s = defClass % (declaration.name)
# #         elif t == type (dw.variable_wrapper.variable_t()):
# #             pass
# #             
# #         elif t == type (dw.calldef_wrapper.destructor_t()):
# #             pass
# #             
# #         elif t == type (dw.calldef_wrapper.member_function_t()):
# #             sig,param = getSignature ( declaration )
# #             s = defFunction % (declaration.name)
# #             for p in param:
# #                 s = s + "\n  :param " + p[0]
# #                 s = s + "\n  :type " + p[1]
# #                 
# #             #s = defFunction % ( declaration.name)
# #             
# #         elif t == type (dw.calldef_wrapper.constructor_t()):
# #             sig,param = getSignature ( declaration )
# #             s = defConstructor % (declaration.name, sig)
# #             for k,v in param:
# #                 s+="\n   :type "+k+": "+v
# #             
# #             print s
# #         elif t == type (dw.enumeration_wrapper.enumeration_t()):
# #             pass    
# #         elif t == type (dw.typedef_wrapper.typedef_t()):
# #             pass    
# #         else :
# #             print "?????", t #if t == type (dw.class_wrapper.class_t):
# #             print type(dw.class_wrapper.class_t())
# #         
# #         print s
# #                  
# #         ## note the gccxml uses a 1 based line scheme whereas we are using python arrays - 0 based
# #         ## so we need to subtract 1 from the line number.
# #         
# #         ## then we need to subtract another 1 to put us on the line above the function/class etc defination
# #         
# #         ## AND to make it worse, Ogre coding style often has the opening brace on the line AFTER the defination name
# #         ## and gccxml gives us the line number with the brace - so we need to look back even further
# #                 
# # 
