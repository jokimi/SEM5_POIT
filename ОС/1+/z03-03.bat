@echo off
setlocal

rem Creating "TXT" directory, if it doesn't exist
if not exist "TXT" (
    mkdir "TXT"
)

set extension=%1%
echo file extension: %extension%

if "%extension%"=="" (
	echo this file's extension: %0%
) 
if "%extension%"=="txt" (

		move *.txt "TXT"
		echo file moved
)

pause