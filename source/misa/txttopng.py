# -*- coding: utf-8 -*-
  
import os
import sys 
from PIL import Image, ImageFont, ImageDraw

if __name__=="__main__":

    # icon
    fileName = sys.argv[1] 
    outPath = sys.argv[2]

    # file name
    dotIndex = str.rfind( fileName, '.' )
    slashIndex = str.rfind( fileName, '\\')

    rawFileName = fileName[ slashIndex+1:dotIndex ]
    
    print(rawFileName)
    print(outPath)

    text = rawFileName
    
    im = Image.new("RGB", (32, 32), (255, 255, 255) )
    dr = ImageDraw.Draw(im)
    dr.rectangle((0,0,31,31),outline=(0,0,0))
    font = ImageFont.truetype(os.path.join("fonts", "cour.ttf"), 10)
    # font = ImageFont.truetype(os.path.join("fonts", "arial.ttf"), 8)
    # font = ImageFont.truetype(os.path.join("fonts", "times.ttf"), 8)

    lineC = 5
    lineHeight = 9
    for i in range( 0, int(len(text)/lineC) + 1 ):
        dr.text((0, i*lineHeight), text[ i * lineC : min( (i + 1 )*lineC, len(text) ) ], font=font, fill="#000000")
 
    # im.show()
    # im.save( "t.png" )
    im.save( outPath + "\\" + rawFileName + ".png" )