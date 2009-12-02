from PIL import Image
from PIL import ImageChops

import os

size = 400, 300
base = os.path.abspath ( "../screenshots" )
thumbbase = os.path.abspath ( "../screenshots/thumbs" )
      
files = os.listdir ( base )
for f in files:
   file, ext = os.path.splitext(f)
   print file,ext
   if ext == ".jpg":
      im = Image.open(os.path.join(base,f))
      im.thumbnail(size, Image.ANTIALIAS)
      im.save(os.path.join(thumbbase,file + "_thumbnail.jpg"), "JPEG")
    