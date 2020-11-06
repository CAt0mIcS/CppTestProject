@echo off

IF NOT EXIST %~dp0\build (
	echo Build Directory does not exist, creating it...
	mkdir build
) ELSE (
	echo Build Directory %~dp0build found
)

cd build

echo Starting CMake
call cmake ..
echo Cmake Finnished
PAUSE