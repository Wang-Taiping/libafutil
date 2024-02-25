@echo off

set CURL_VERSION=8.6.0
set CURL_TAG=curl-8_6_0

echo Initializing curl-%CURL_VERSION%.

if not exist libafutil.sln (
	echo Initialization failed: The working directory is incorrect, and the repository root directory should be used as the working directory.
	exit 1
)

where git >nul 2>nul
if %ERRORLEVEL% neq 0 (
	echo Initialization failed: The git command is required, please install Git.
	exit 1
)

rmdir /s /q libcurl\curl >nul 2>nul
git clone --branch %CURL_TAG% --depth=1 https://github.com/curl/curl libcurl\curl >nul 2>nul

if exist libcurl\curl (
	cd libcurl\curl
	cmd /c .\buildconf.bat >nul 2>nul
	echo Initialization success!
	exit 0
)
else (
	echo Initialization failed: Network errors.
	exit 1
)