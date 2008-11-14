"""
extracting from C++ doxygen documented file
Author G.D.

Updates for Ogre handling by AJM - added remarks/note/par
jan 03 06 - fix bug, replace single leading \ with \\ in clear_str
"""

class doc_extractor:
    """
    extracts doxigen styled documentation from source
    or generates from description
    """
    def __init__(self, startswith="", outfile="documentation.dox"):
        #for caching source
        self.file_name  = None
        self.source = None
        self.startswith = startswith
#         self.outfile = open ( outfile, 'w' ) # create the file for writing (truncates existing)
        
    def __call__(self, declaration):
# # #         if not self.startswith in declaration.decl_string:
# #         if not declaration.demangled.startswith(self.startswith):
# #             print "DOC: not in scope", declaration.decl_string 
# #             print "name:", declaration.name
# #             print "demangled:", declaration.demangled
# # #             print "type:", declaration.type
# # #             if "Ogre" in declaration.decl_string : sys.exit()
# #             return ""
        try:    # some types such as the base namespace don't have 'location'
            if self.file_name != declaration.location.file_name:            
                self.file_name = declaration.location.file_name
                self.source = open(declaration.location.file_name).readlines()
        except:
            print "DOC without location:", declaration
            return ""
            
        find_block_end = False
        failed = 0
        doc_lines = []
        if declaration.documentation:
            if len(declaration.documentation) > 2:
                basedoc = declaration.documentation + "\\\n"
        else:
            basedoc = ""
        ## note the gccxml uses a 1 based line scheme whereas we are using python arrays - 0 based
        ## so we need to subtract 1 from the line number.
        
        ## then we need to subtract another 1 to put us on the line above the function/class etc defination
        
        ## AND to make it worse, Ogre coding style often has the opening brace on the line AFTER the defination name
        ## and gccxml gives us the line number with the brace - so we need to look back even further
        
        ## lets look for a single comment '///'  We'll try upto 2 lines above
        try:
            for lcount in xrange(declaration.location.line-1, declaration.location.line-4 , -1):
                line = self.source[lcount]
                if line.lstrip()[:3] == '///':
                    str_ = clear_str(line)
                    if basedoc:
                        retvalue = '"' + basedoc +  str_[:-2] + '"' # remove the \n from the single line comment
                    else:
                        retvalue = '"' + str_[:-2] + '"' # remove the \n from the single line comment
                    #print "Extracted Doc String (short) ", retvalue
                    return retvalue
                    
            ## if that didn't work we'll look for a block of comments
            
            for lcount in xrange(declaration.location.line - 1, -1, -1):
                line = self.source[lcount]
                #print "Checking ", lcount, line
                if not find_block_end:
                    try:
                        if line.rstrip()[-2:] == "*/":
                            find_block_end = True
                    except:
                        pass
                if  find_block_end:
                    try:
                        if line.lstrip()[:2] == "/*":
                            find_block_end = False
                    except:
                        pass
                final_str = clear_str(line)
                if not find_block_end and code(line):
                    ## note that ogre code tends to have the opening function/class bracket on the 'next' line 
                    ## from the 'name' -so we may need to backup one more line to fine doc string..
                    failed +=1
                    final_str = ""  ## make sure we don't add the extra code line
                    if failed > 2:
                        break
                if final_str:
                    doc_lines.insert(0, clear_str(final_str))
        except:
            if not basedoc:
                return ""
        ret ="" 
        if doc_lines:
            doc_lines = remove_leading_blanks ( doc_lines )
            #print "Extracted Doc String for:",  declaration, "[", len(doc_lines),"]"
            ## we need to cope with strings longer than 2048 for MSVC
            ret =  "\\\n".join(doc_lines) 
            if len ( ret ) > 1700:  ## just to be safe and adjust for line end chars etc..
                ret = ""
                length = 1
                for line in doc_lines:
                    ret = ret + line
                    length += len ( line )
                    if length < 1500 :  # again lets be conservative
                        ret = ret + "\\\n"
                    else :
                        ret = ret + '"' + "\n" + '"'    # we close the original 'quote', move to a new line and open a new quote
                        length = 1
        ## Py++ doesn't like non ascii characters in the doc string
        ## class_declaration.py - _generate_constructor - return ( ''.join( result ), used_init )
        newret = ""
#         print "DOC:  GOT SOMETHING", ret
        for c in ret:
            if ord(c) >127:
                c = ' '
            newret = newret + c
        if len(basedoc) >1 or len(newret)>1:
# #             self.outfile.write("***============================*****\n" + declaration.decl_string + "\n")
# #             self.outfile.write( str(len(basedoc)) + "  " + str(len(newret)) + "\n" )
# #             self.outfile.write( newret )      
            return '"' + basedoc + newret.lstrip() + '"'            
        else: return ""

def remove_leading_blanks ( docin ):
    """
    remove any initial lines that are '\n\'
    """  
    returnlist=[]
    
    if docin[0].strip() != "\\n":
# #         print "** ", docin[0]
        return docin # nothing to fix so return the original list..
        
    fixed = False
    for line in docin:
        if not fixed:
            if line.strip() != "\\n":
                fixed = True 
        if fixed:
            returnlist.append(line)
    return returnlist
                
           
def get_generic_doc(declaration):
    """
    generate call information about function or method
    """
    try:
        return '"' + "Help on %s\n" % str(declaration) + '"'
    except:
        pass
    
    return ''
                


def code(str):
    """
    detect str is code?
    """
    try:
        beg = str.lstrip()[:2]
        return  beg != "//" and beg != "/*"
    except:
        pass
    return False

           
def clear_str(_str):
    """
    replace */! by Space and \breaf, \fn, \param, ...
    """
    def clean ( _str, sym, change2 = ""): 
        return _str.replace(sym, change2)
        
        
    _str = clean(_str, "::", ".")     ## make it more python accurate 
    _str = clean(_str, "->", ".") 
#     _str = clean(_str, "@param", "Param: ")
    _str = clean(_str, "\\param", "@param")
    _str = clean(_str, "@ingroup", "@group")
    _str = clean(_str, "\\ingroup", "@group")
#     _str = clean(_str, "@return", "It return")
    _str = clean(_str, "\\return", "@return")
    _str = clean(_str, "\\Return", "@return")
#     _str = clean(_str, "@note", "Note: ")
    _str = clean(_str, "@remarks", "@see")
    _str = clean(_str, "\\remarks", "@see")
    _str = clean(_str, "\\Remarks", "@see")
#     _str = clean(_str, "@see", "See: ")
    _str = clean(_str, "\\see", "@see")
    _str = clean(_str, "@ref", "@see")
    _str = clean(_str, "\\ref", "@see")
    _str = clean(_str, "@copydoc", "Ref: ")
    _str = clean(_str, "\\verbatim", "" )
    _str = clean(_str, "\\endverbatim", "" )
    
    
    _str = clean(_str, "\\sa", "@see")   # comment _string in OgreNewt
    _str = clean(_str, "\\codeblock", "::")    
    _str = clean(_str, "\\code", "::")    
    _str = clean(_str, "\\endcode", "\\n")    
    _str = clean(_str, "@codeblock", "\\n")   # endsure a new line to end the code block 
    _str = clean(_str, "@code", "::")   # using literal code blocks 
    
#     _str = clean(_str, "@par", "")    ## it will get a single blank line by default -- breaks @param...
    _str = clean(_str, "\r\n", "\\n")   ## for linux build
    _str = clean(_str, "\\par", "")    ## it will get a single blank line by default
    _str = clean(_str, "\n", "\\n") 
    _str = clean(_str, "\\p", "")     ## cegui comments
    _str = clean(_str, "\\li", "  * ")     ## ode comments
    _str = clean(_str, "\\exception", "@exception") 
    _str = clean(_str, "\\a", "")     ## cegui
    
    _str = clean(_str, "\\warning", "@Warning: ")     ## nxPhysics
    _str = clean(_str, "\\include", "Include: ")     ## nxPhysics
    _str = clean(_str, "\\image", "Image: ")     ## nxPhysics
    
    _str = clean(_str, "\\examples", "Examples: ")     ## nxOgre
    _str = clean(_str, "\\default", "Default: ")     ## nxOgre

    _str = clean(_str, "\\version", "@version ")     ## cegui
    _str = clean(_str, "\\date", "@date ")     #
    _str = clean(_str, "\\todo", "@todo ")     ## 
    _str = clean(_str, "\\bug", "@Bug ")     ## 
    _str = clean(_str, "\\author", "@Author ")     ## 
    
    
    ## now clean up the rest
    _str = reduce(clean, [_str, '/', "\\brief", '\\fn',
         "@brief", "@fn", '"', "@{", "\\c", "\\a"]) ## somtimes there are '"' in the doc strings and other "\\"...
#     return _str.lstrip()
#     return "  " + _str.lstrip()

    # another fix to remove stray escape sequences - only new lines allowed
    # last character should never be a backslash, however lets fix it just in case...
    # and we need to make a new copy because strings are immutable...
    newStr=""
    for x in range ( len ( _str) -1  ): # handle upto 2nd to last character
        if _str[x] == '\\' and _str[x+1] != 'n':
# #             print "fixing stray escape sequence", x, _str
            newStr += ' ' 
        else:
            newStr += _str[x]                   
    x = len (_str) -1 # need to handle last character
    if _str[x] == '\\' :
# #         print "fixing stray escape sequence EOL", _str
        newStr += ' ' 
    else:
        newStr += _str[x]                   
    return newStr  ## no removale of white space so Epytext can handle sections etc

    
def make_epytext ( str ):
    """ turn this string into something more Epytext compatible """
    knownfields = ['param']
    
if __name__ == '__main__':
    class loc:
        def __init__(self, f, l):
            self.file_name = f
            self.line = l
            
    class x_decl:
        def __init__(self, str, file_name, line):
            self.str  = str
            self.location = loc(file_name, line)
            self.decl_string = ""
            
        def __str__(self):
            return self.str
        
    print doc_extractor("")(x_decl("myfunc(int x, int y)","c:/development/ocvs/ogrenew/ogremain/include/OgreSceneManager.h",218))
    print doc_extractor("")(x_decl("","c:/development/ocvs/ogrenew/ogremain/include/OgreSceneManager.h",223))
    
    print doc_extractor("")(x_decl("","c:/development/CEGUI-0.5.0/include/CEGUIEvent.h",139))
