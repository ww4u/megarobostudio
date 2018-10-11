@echo off
echo ImageMagick fix libpng warning: iCCP: Not recognizing known sRGB profile ......
echo Search PNG in subdirs and process ...
set fn=D:\Program Files\ImageMagick-7.0.8-Q8\magick.exe
for /f "tokens=*" %%i in ('dir/s/b *.png') do (
"%fn%" "%%i" -strip "%%i"
echo %%i
)
pause