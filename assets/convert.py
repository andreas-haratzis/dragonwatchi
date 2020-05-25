#!/usr/bin/python
from PIL import Image
import glob
import os
import re

h_file = open('../dragonwatchi/sprites.h', 'w')  
cpp_file = open('../dragonwatchi/sprites.cpp', 'w')  

h_file.write('#ifndef SPRITES_H\n')
h_file.write('#define SPRITES_H\n\n')
h_file.write('#include <HyperDisplay_KWH018ST01_4WSPI.h>\n')
h_file.write('// Auto generated by assets/generate.py\n\n')
h_file.write('struct Sprite { uint16_t width; uint16_t height; const ILI9163C_color_18_t* pixels; };\n\n')

cpp_file.write('#include "sprites.h"\n\n')

for file in sorted(glob.glob('*.png')):
    name = os.path.splitext(file)[0]
    sprite_name = 'sprite_' + name 

    img = Image.open(file)
    img = img.rotate(90, expand=True)
    #img.show()
    
    h_file.write('const extern PROGMEM Sprite ' + sprite_name + ';\n')

    cpp_file.write('const static PROGMEM ILI9163C_color_18_t ' + sprite_name + '_pixels[' + str(img.width) + '*' + str(img.height) + '] = {\n')
    for y in range(img.height):
        for x in range(img.width):
            pixel = img.getpixel((x,y))
            cpp_file.write('{')
            for i in range(3):
                cpp_file.write(str(pixel[i]))
                if i < len(pixel) - 1:
                    cpp_file.write(',')
            cpp_file.write('},')
                
        cpp_file.write('\n')
            
    cpp_file.write('};\n')
    cpp_file.write('const PROGMEM Sprite ' + sprite_name + '{' + str(img.width) + ',' + str(img.height) + ',' + sprite_name + '_pixels};\n\n')

h_file.write('\n\n')
cpp_file.write('\n\n')

h_file.write('#endif\n')
