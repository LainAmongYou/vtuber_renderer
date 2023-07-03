@echo off

:: Project directory
SET HOST_DIR=%~dp0
SET HOST_DIR=%HOST_DIR:~0,-1%

:: TODO: swap between debug and release
call %HOST_DIR%\build\Debug\Vtuber_Renderer.exe