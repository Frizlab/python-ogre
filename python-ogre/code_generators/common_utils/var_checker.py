"""
Check of the variable is being 'set' in the header file
This is BAD for static const vars as the compile fails in Linux
"""

class var_checker:
    """
    Checks for '=' in var defination line
    """
    def __init__(self):
        #for caching source
        self.file_name  = None
        self.source = None
        
    def __call__(self, declaration):
        print "var_checker called with:", declaration
        if not declaration.location.file_name :
            print __file__, ": location file name is empty", declaration
            return False 
        if self.file_name != declaration.location.file_name:            
            self.file_name = declaration.location.file_name
            try:    
                self.source = open(self.file_name).readlines()
            except:
                print __file__, ": Unable to open ", self.file_name, "source is unknown", declaration
                self.file_name= None # ensure we try again next time
                return False
            
        ## note the gccxml uses a 1 based line scheme whereas we are using python arrays - 0 based
        ## so we need to subtract 1 from the line number.
#         print declaration.location.line, len(self.source)
        try:
            varline = self.source[declaration.location.line-1]
        except IndexError:
            print __file__, ":IndexError ", self.source, declaration.location.line
            return False # Bug showed up with Caelum where decl location was bigger than source ??
        except TypeError:
            print __file__, ":TypeError ", self.source, declaration.location.line
            return False
#         print "Checker info:",declaration.location.line, declaration.location.file_name
#         print "Checker Line:", varline
        if not '=' in varline:
            return False
        # do a simple check to ensure the '=' hasn't been commented out
        for char in varline:
            if char =='/' or char =='\\':
                return False
            elif char == '=':
                return True
        return False
        
