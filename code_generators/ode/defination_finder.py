"""
Need to lookup the real defination of the var by going back to the source
"""

class def_finder:
    """
    Checks for '=' in var defination line
    """
    def __init__(self):
        #for caching source
        self.file_name  = None
        self.source = None
        
    def __call__(self, declaration, arg_position):
        #print "var_checker called with:", declaration
        try:    
            if self.file_name != declaration.location.file_name:            
                self.file_name = declaration.location.file_name
                self.source = open(declaration.location.file_name).readlines()
        except:
            print "Def Finder called without location:", declaration
            return ""
            
        ## note the gccxml uses a 1 based line scheme whereas we are using python arrays - 0 based
        ## so we need to subtract 1 from the line number.
        endlocation = declaration.location.line-1
        startlocation = endlocation

        ##
        ## the ODE source likes to split definations across multiple lines so we need to backup until we
        ## find the opening brace, then make a single line from rest
        ##
        print "Starting with:", self.source[startlocation]
        
        ## lets look for the function name and a '('
        while (not "(" in self.source[startlocation] and 
            declaration.name not in self.source[startlocation]) or\
                self.source[startlocation].lstrip()[0] == '{':
            ## need to backup as defs are across multiple line
            startlocation -=1
            print"Backed up to:", self.source[startlocation].lstrip()
             
        varline = ""   
        for x in range (startlocation, endlocation+1):
            varline = varline + self.source[x]
        print "NEW LINE", varline
        # find open brace
        sb = varline.find( "(" )
        eb = varline.find( ")" )
        s = varline[sb+1:eb]
        print "Checker Args:", s
        vars = s.split(",")
        print "Checker Arg:", vars[arg_position]
        return vars[arg_position]
        
