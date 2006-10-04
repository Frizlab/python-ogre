#I think it is possible to create code that will do that automaticly.

aliases = {

#      "std::map<std::string, std::string>" :
#      	"ParamList",

#     "class std::multimap<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,struct std::less<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > >,class std::allocator<struct std::pair<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > const ,class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> > > > > ":
#       "MapParamList",
#     "std::multimap<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >" :
#       "MMapParam"       
    "map<std::string, std::string, std::less<std::string>, std::allocator<std::pair<std::string const, std::string> > >"
      : "ParamList"
      

}

#This files will be added to the generated code:
header_files = [
	"OIS.h",
   ]
      


huge_classes = [

]

