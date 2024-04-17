cd out
cmake ..
cmake --build . --config Debug --target Case-Engine
cd ..
copy source\DLLs\SDL2.dll x64\Debug\SDL2.dll 
pause