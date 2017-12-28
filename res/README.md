
### 文件格式

字段              | 字节数
------------------| ---------
PXS\0             | 4 byte (byte)
mesh array length | 4 byte (int)
mesh (N)          |
obj array length  | 4 byte (int)
obj (N)           |


### Mesh 格式

字段              | 字节数
------------------| ---------
type              | 2 byte (short)
vertices length   | 4 byte (int)
XYZ (N)           | 4 byte (float) + 4 byte (float) + 4 byte (float) - N 个
indices length    | 4 byte (int)
indice (N)        | 2 byte (short) - N 个


### Box (Obj) 格式

字段              | 字节数
------------------| ---------
type              | 2 byte (short)
position X        | 4 byte (float)
position Y        | 4 byte (float)
position Z        | 4 byte (float)
rotation X        | 4 byte (float)
rotation Y        | 4 byte (float)
rotation Z        | 4 byte (float)
layer             | 1 byte (byte)
half extents X    | 4 byte (float)
half extents Y    | 4 byte (float)
half extents Z    | 4 byte (float)


### Capsule (Obj) 格式

字段              | 字节数
------------------| ---------
type              | 2 byte (short)
position X        | 4 byte (float)
position Y        | 4 byte (float)
position Z        | 4 byte (float)
rotation X        | 4 byte (float)
rotation Y        | 4 byte (float)
rotation Z        | 4 byte (float)
layer             | 1 byte (byte)
radius            | 4 byte (float)
half height       | 4 byte (float)


### Mesh (Obj) 格式

字段              | 字节数
------------------| ---------
type              | 2 byte (short)
position X        | 4 byte (float)
position Y        | 4 byte (float)
position Z        | 4 byte (float)
rotation X        | 4 byte (float)
rotation Y        | 4 byte (float)
rotation Z        | 4 byte (float)
layer             | 1 byte (byte)
scale X           | 4 byte (float)
scale Y           | 4 byte (float)
scale Z           | 4 byte (float)
mesh index        | 4 byte (int)


### Terrain (Obj) 格式

字段              | 字节数
------------------| ---------
type              | 2 byte (short)
position X        | 4 byte (float)
position Y        | 4 byte (float)
position Z        | 4 byte (float)
rotation X        | 4 byte (float)
rotation Y        | 4 byte (float)
rotation Z        | 4 byte (float)
layer             | 1 byte (byte)
size X            | 4 byte (float)
size Y            | 4 byte (float)
size Z            | 4 byte (float)
height map size D | 4 byte (int)
D x D value       | 4 byte (float) - D x D 个
