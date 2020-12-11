if not defined DevEnvDir (
	call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)
set initdir="%cd%"
set batdir=%~dp0
set bindir="%batdir%..\bin\"
if not exist %bindir% (
	mkdir %bindir%
)
cd %bindir%
cmake -G "NMake Makefiles" ..
nmake
cd %initdir%
