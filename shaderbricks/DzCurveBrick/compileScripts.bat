@ECHO OFF

@REM	%1 - Platform Name (Win32/x64)

@REM Create local variables for readability
set platform=%1

cd ./scripts/
dir /B *.sl > scriptlist.txt
..\..\..\..\bin\%platform%\dpc -mode script -o dpcscript.cpp -f scriptlist.txt