call _load_env.bat

if not exist boost_1_64_0.7z %_curl% -o boost_1_64_0.7z -k -L https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.7z
if not exist boost_1_64_0 %_7z% x boost_1_64_0.7z -y -aos -o"."

cd boost_1_64_0
call .\bootstrap.bat

echo import option ; > %CUR_PATH%\boost_1_64_0\project-config.jam
echo using msvc ; >> %CUR_PATH%\boost_1_64_0\project-config.jam
echo using python : 3.6 : C:\\Python36 ; >> %CUR_PATH%\boost_1_64_0\project-config.jam
echo option.set keep-going : false ; >> %CUR_PATH%\boost_1_64_0\project-config.jam

%CUR_PATH%\boost_1_64_0\b2 --toolset=msvc-14.0 variant=release threading=multi link=static runtime-link=static address-model=64 --with-python --with-program_options --with-filesystem --with-system --with-log --with-thread --with-serialization
%CUR_PATH%\boost_1_64_0\b2 --toolset=msvc-14.0 variant=debug threading=multi link=static runtime-link=static address-model=64 --with-python --with-program_options --with-filesystem --with-system --with-log --with-thread --with-serialization

xcopy /Y /Q /S boost ..\..\include\boost\
copy /Y stage\lib\*.lib ..\..\lib_win
move /Y ..\..\lib_win\libboost_python3-vc140-mt-s-1_64.lib ..\..\lib_win\libboost_python-vc140-mt-s-1_64.lib
move /Y ..\..\lib_win\libboost_python3-vc140-mt-sgd-1_64.lib ..\..\lib_win\libboost_python-vc140-mt-sgd-1_64.lib

cd ..\