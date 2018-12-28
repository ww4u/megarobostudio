::@echo off
cd labview
for /f "tokens=*" %%i in ('dir /s/b *.vi') do (
python ..\txttopng.py %%i ..\image
echo %%i
)
pause