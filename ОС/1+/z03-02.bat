@echo off
setlocal

rem Creating "TXT" directory, if it doesn't exist
if not exist "TXT" (
    mkdir "TXT"
)

rem Moving .txt files to "TXT" directory
move *.txt "TXT"

echo All .txt files were moved to "TXT" directory
pause