@echo off
set PATH=%PATH;..\Tools\GetRegions\
@echo on
GetRegions.exe %1 -c -s -x__ -x_ -xQ_ -xPIT_ -oSource\Profiler\region.c
@echo off
REM set PATH=%PATH;\Keil_v5\ARM\ARMCC\bin\
REM @echo on
REM del /F %1.dis.txt
REM C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --text -c --output %1.dis.txt %1  
