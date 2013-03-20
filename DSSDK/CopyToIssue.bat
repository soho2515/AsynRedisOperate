@echo off
::
:: This batch file is used to copy S8SDK issue.
:: Created by houzhifang in 2010 06 22
::

:: FIND THE SDK DIRECTORY
:: Prefer the most local sdk dir,
:: so order these checks such that local dirs set the variable last.
set SDK_DIR=not_found
if exist ..\..\04_CoreLib\DSSDK set SDK_DIR=..\..\04_CoreLib\DSSDK
if (%SDK_DIR%)==(not_found) goto :ERROR_NO_SDK_DIR

:: Create include and lib directory if they are missing
if not exist %SDK_DIR%\include md %SDK_DIR%\include
if not exist %SDK_DIR%\lib md %SDK_DIR%\lib

echo (only copying new or modified files)

if exist .\lib\*.lib               xcopy /D /F /I /R /Y .\lib\*.lib              %SDK_DIR%\lib
if exist .\include\*.h             xcopy /D /F /I /R /Y .\include\*.h            %SDK_DIR%\include
if exist .\Lualib\include\*.h      xcopy /D /F /I /R /Y .\Lualib\include\*.h     %SDK_DIR%\include
if exist .\Lualib\include\*.inl      xcopy /D /F /I /R /Y .\Lualib\include\*.inl     %SDK_DIR%\include


:: MARK FILES READ ONLY
if exist %SDK_DIR%\include attrib +R %SDK_DIR%\include\*
if exist %SDK_DIR%\lib attrib +R %SDK_DIR%\lib\*

echo 请按任意键结束
pause>nul
exit /b

:==ERROR_NO_SDK_DIR
echo %0: fatal error: Cannot locate SDK directory.
echo 请按任意键结束
pause>nul
exit 1
