from pygccxml import declarations

# class
# constructor
# member function
# descructors
# variables

# 
# output Arrays
#
mainOutput = []
headerOutput = []
footerOutput = []
templateOutput = []
handWrapperOutput = []

def addOutput ( line, output = mainOutput ):
    output.append ( line )
    
def addHeader ( line ):
    headerOutput.append ( line )
#
# Handle the concept of public/protected access attributes
#
currentAccess = ""
def checkAccess( newAccess = "" ):
    global currentAccess
    if newAccess == currentAccess: 
        return
    currentAccess = newAccess
    mainOutput.append(newAccess + ':' )
    
def clearAccess ():
    global currentAccess
    currentAccess = ""
    
def setAccess ( access ):
    global currentAccess
    currentAccess = access

#
# handle various attributes = no intelligence, just do it
#

def is_virtual ( cls ):
    try:
        return cls.has_vtable
    except:
        if cls.virtuality != declarations.VIRTUALITY_TYPES.NOT_VIRTUAL:
            return True
    return False

def check_virtual ( defination ):
    if is_virtual(defination):
        return "virtual "
    return "" 
    
classClose = ""
def process_class ( cls, base='Ogre' ):
    global mainOutput, classClose
    outString = ""
    #if check_virtual ( cls ):
    #    outString="\\VIRTUAL CLASS\n"
    classClose = "};"
    if cls.parent.name != base:
        outString += "namespace " + cls.parent.name + " {\n"
        classClose += "\n};"
    outString +=  "class "  + cls.name
    
    first = 0 
    for b in cls.bases:
        r = b.related_class
        if ("Allocated" not in r.name 
            and not b.access_type == "private"
            and "Singleton" not in r.name
            and not is_virtual ( r ) ) :
                if first == 0:
                    outString += " : " + b.access_type + " " + r.name + " "
                if first > 0:
                    outString += " ," + b.access_type + " " + r.name                     
                first += 1
    outString += "\n{"
    mainOutput.append ( outString )
    
    # print mainOutput
    return outString
    
def process_class_close():
    mainOutput.append ( classClose )
    
def process_destructor ( destructor, class_name ):
    global mainOutput
    checkAccess ( destructor.access_type )
    outString = "\t" + check_virtual ( destructor )
    outString += destructor.decl_string
    mainOutput.append ( outString )
    
def process_constructor ( constructor, class_name ):
    global mainOutput
    checkAccess ( constructor.access_type )
    outString = "\t" + check_virtual ( constructor )
    outString += class_name + " ("
    count = 0
    for v in constructor.arguments:
        if count > 0:
            outString += " ,"
        outString += v.type.decl_string + " " + v.name
        count += 1
    if count == 0:
        outString += " void"                     
    outString +=  " );"
    mainOutput.append ( outString )
    #print "RECEIVED", constructor
    #print outString
    #print
    return outString
    
def process_function ( function ):
    global mainOutput
    checkAccess ( function.access_type )
    outString = "\t" + check_virtual ( function )
    outString += str(function.return_type) + " " + function.name + " ("     #the function name
    count = 0
    for v in function.arguments:   # handle any arguments
        if count > 0:
            outString += ", "
        outString += v.type.decl_string + " " + v.name + " "
        count += 1
        if count == 0:
            outString += " void ",   # put void in if no arguments to be proper... 
    outString += " )"
    if function.virtuality == declarations.VIRTUALITY_TYPES.PURE_VIRTUAL:
        outString += " = 0"
    outString += ";"
    mainOutput.append ( outString )
    #print "Returning:", mainOutput
    return outString

def process_template ( cls ):
    outString =  "%template ("+ cls.alias+") Ogre::"+cls.name+";"
    templateOutput.append ( outString )
    return outString
    
def getOutput ():
    output = ""
    for a in headerOutput, handWrapperOutput, mainOutput, templateOutput, footerOutput:
        output += "\n".join ( a )
        output += "\n"
    return output
    
def write ( filename = None ):
    if filename:
        try:
            f= open( filename, 'w' )
            f.write ( getOutput() )
            f.close()
        except:
            print "ERROR writing output to", filename
            raise
     
        