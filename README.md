# PhysxWrap

### 编译说明

#### 关于physx源码包

physx是官网注册后开源，因此请自行按照官网说明获得physx源码包。

#### window

1. 下载physx源码包，将其拷贝至deps\\_packages\\PhysX-3.4-master.zip
1. 打开`适用于 VS 2017 的 x64 本机工具命令提示`控制台
1. 执行下列语句，获取依赖库和工程文件：
  ```dos
  git submodule update --init --force -- "tools/build"
  cd deps\lib_win
  gen.bat
  cd ..\..\build
  premake5-win.bat
  ```  
1. 打开PhysxWrap.sln，编译

#### linux

1. 下载physx源码包，将其拷贝至deps/_packages/docker-build/physx/PhysX-3.4-master.zip
1. 执行下列语句，获取依赖库并编译：
  ```bash
  git submodule update --init --force -- "tools/build"
  cd deps/lib_linux
  ./gen.sh
  cd ../../build
  ./premake5-linux.sh
  make
  ```


### TODO

1. 增加设置密度接口
1. 增加碰撞分组接口
1. 增加射线检测接口
