@echo off
if not exist libafutil.sln (
	echo Folder error.
	exit 1
)
rmdir /s /q libcurl\curl >nul 2>nul
git clone --branch curl-8_6_0 --depth=1 https://github.com/curl/curl libcurl\curl
cmd /c libcurl\curl\buildconf.bat