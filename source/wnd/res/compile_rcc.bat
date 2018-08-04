cd .
set rccpath=D:\Qt5\5.10.0\mingw53_32\bin

:: for white
%rccpath%\rcc.exe -binary res.qrc -o res.rcc

:: compile for black
:: rename white
cd image
for %%d in ( icon icon2 icon3 ) do ( ren %%d w_%%d )

:: rename black
ren b_icon icon
ren b_icon2 icon2
ren b_icon3 icon3

cd ..
%rccpath%\rcc.exe -binary res.qrc -o black.rcc

:: rename to black
cd image
for %%d in ( icon icon2 icon3 ) do ( ren %%d b_%%d )

:: rename to white
ren w_icon icon
ren w_icon2 icon2
ren w_icon3 icon3

cd ..

set dst=G:\work\mc\develope\installer\style

for %%d in ( res.rcc black.rcc ) do ( copy %%d %dst%\%%d ) 

set dst=G:\work\mc\develope\source\wnd\build-mc-mingw-Debug\debug\style
for %%d in ( res.rcc black.rcc ) do ( copy %%d %dst%\%%d ) 