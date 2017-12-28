set CUR_PATH=%~dp0

REM 设置VS路径
set _devenv="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\devenv.com"

REM 设置MSBuild路径
set PATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\amd64;%PATH%

REM 设置curl路径
set _curl=%CUR_PATH%\..\..\tools\build\curl-win\curl.exe

REM 设置7z路径
set _7z=%CUR_PATH%\..\..\tools\build\7z-win\7z.exe

REM 设置tar路径
set _tar=%CUR_PATH%\..\..\tools\build\tar-win\WinTar_1_0_2.exe

REM 设置sed路径
set _sed=%CUR_PATH%\..\..\tools\build\sed-win\sed.exe
