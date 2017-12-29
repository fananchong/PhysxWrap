import glob
import struct
from PIL import Image

if __name__ == "__main__":
    files = glob.glob('*.gif')
    for file in files:
        print("start process file, file = %s" % (file))
        newfile = file[ : len(file) - len('.gif') ] + ".data"
        
        image = Image.open(file).convert("L")
        columns = image.size[0]
        rows = image.size[1]
        
        f = open(newfile, 'wb')
        f.write(struct.pack('I', columns))
        f.write(struct.pack('I', rows))
        for i in range(columns * rows):
            f.write(struct.pack('H', image.im[i]))
        f.close()
        