call _load_env.bat

set CURDIR=%~dp0

if exist PhysX-3.4-master.zip %_7z% x PhysX-3.4-master.zip -y -aos -o"."

cd %CURDIR%\PhysX-3.4-master\PhysX_3.4\Source\compiler\vc14win64

%_devenv% PhysX.sln /build "Debug|x64" /Out build_debug.log
%_devenv% PhysX.sln /build "Release|x64" /Out build_release.log

copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Lib\vc14win64\* %CURDIR%\..\lib_win
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\lib\vc14win64\* %CURDIR%\..\lib_win


if not exist %CURDIR%\..\include\physx\PhysX_3.4\ mkdir %CURDIR%\..\include\physx\PhysX_3.4\
if not exist %CURDIR%\..\include\physx\PxShared\ mkdir %CURDIR%\..\include\physx\PxShared\
xcopy /Y /S %CURDIR%\PhysX-3.4-master\PhysX_3.4\Include %CURDIR%\..\include\physx\PhysX_3.4
xcopy /Y /S %CURDIR%\PhysX-3.4-master\PxShared\include %CURDIR%\..\include\physx\PxShared

if not exist %CURDIR%\..\..\bin\Debug\ mkdir %CURDIR%\..\..\bin\Debug\
if not exist %CURDIR%\..\..\bin\Release\ mkdir %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3_x64.dll %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxFoundation_x64.dll %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxPvdSDK_x64.dll %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3Common_x64.dll %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3Cooking_x64.dll %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\nvToolsExt64_1.dll %CURDIR%\..\..\bin\Release\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3DEBUG_x64.dll %CURDIR%\..\..\bin\Debug\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxFoundationDEBUG_x64.dll %CURDIR%\..\..\bin\Debug\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxPvdSDKDEBUG_x64.dll %CURDIR%\..\..\bin\Debug\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3CommonDEBUG_x64.dll %CURDIR%\..\..\bin\Debug\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3CookingDEBUG_x64.dll %CURDIR%\..\..\bin\Debug\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\nvToolsExt64_1.dll %CURDIR%\..\..\bin\Debug\

copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxFoundation_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxPvdSDK_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3Common_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3Cooking_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3DEBUG_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxFoundationDEBUG_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PxShared\bin\vc14win64\PxPvdSDKDEBUG_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3CommonDEBUG_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\PhysX3CookingDEBUG_x64.dll %CURDIR%\..\..\tools\py_client\
copy /Y %CURDIR%\PhysX-3.4-master\PhysX_3.4\Bin\vc14win64\nvToolsExt64_1.dll %CURDIR%\..\..\tools\py_client\

cd %CURDIR%
