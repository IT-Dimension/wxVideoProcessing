rem Если есть переменная WXWIN, то норм, иначе выкачать
rem Если есть OPENCV_DIR ничего не делать, если нету, выкачать и поставить set на ней
rem Проверить в Cmake проекта то же , что и в батнике
@echo off
setlocal EnableDelayedExpansion

ECHO Check and build dependencies before actual building

set currentLoc=%cd%
echo Current location: %currentLoc%"

echo Check WXWIN environment

if defined WXWIN (
	echo WXWIN variable exists and its value is:
	echo !WXWIN!
	echo Check if wxWidgets builded
	
	cd %WXWIN%\build\msw
	if "%1" == "x64" (
		echo Build wxWidgets [Release x64]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=release TARGET_CPU=x64
		echo Build wxWidgets [Debug x64]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=debug TARGET_CPU=x64
	) else (
		echo Build wxWidgets [Release x86]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=release
		echo Build wxWidgets [Debug x86]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=debug
	)
	echo Return to %currentLoc%
	cd %currentLoc%
) else (
	rem выкачать wxWidgets на 2 директории выше и собрать 2 версии
	echo Go to PROJECT_DIR
	cd ../..
	git submodule update --init -- "ThirdParty/wxWidgets"
	
	echo Go to wxWidgets directory
	cd ThirdParty/wxWidgets
	setx WXWIN !cd!
	set WXWIN=!cd!
	
	cd wxWidgets/include/wx/msw
	echo Copying setup0.h to setup.h
	copy setup0.h setup.h
	cd ../../../build/msw
	if "%1" == "x64" (
		echo Build wxWidgets [Release x64]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=release TARGET_CPU=x64
		echo Build wxWidgets [Debug x64]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=debug TARGET_CPU=x64
	) else (
		echo Build wxWidgets [Release x86]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=release
		echo Build wxWidgets [Debug x86]
		nmake -f makefile.vc SHARED=1 UNICODE=1 RUNTIME_LIBS=dynamic BUILD=debug
	)
	echo Return to %currentLoc%
	cd %currentLoc%
)

echo Check OpenCV_DIR environment

if defined OpenCV_DIR (
	echo OpenCV_DIR variable exists and its value is: !OpenCV_DIR!
	cd !OpenCV_DIR!
	echo Checking, if it has dirs 'Win' and 'Win64'
	echo "Architecture: %1"
	if "%1" == "x64" (
		echo Selected architecture %1
		if not exist !OpenCV_DIR!\Win64\ (
			echo "!OpenCV_DIR!\Win64\" not exist, creating
			mkdir Win64 && cd Win64
			echo Building OpenCV for Win64 - !OpenCV_DIR!\Win64
			cmake .. -G"Visual Studio 15 2017 Win64" -DBUILD_JASPER=OFF -DBUILD_JAVA=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_WEBP=OFF -DBUILD_WITH_STATIC_CRT=OFF -DBUILD_opencv_apps=OFF -DWITH_1394=OFF -DWITH_MATLAB=OFF -DWITH_WEBP=OFF
			msbuild OpenCV.sln /m /nologo /p:Configuration=Debug /p:Platform=x64
			msbuild OpenCV.sln /m /nologo /p:Configuration=Release /p:Platform=x64
		) else (
			echo Seems "!OpenCV_DIR!\Win64\" dir exists
		)
	) else (
		if not exist "!OpenCV_DIR!\Win\" (
			echo "!OpenCV_DIR!\Win\" not exist, creating
			mkdir Win && cd Win
			echo Building OpenCV for Win32 - !OpenCV_DIR!\Win
			cmake .. -G"Visual Studio 15 2017" -DBUILD_JASPER=OFF -DBUILD_JAVA=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_WEBP=OFF -DBUILD_WITH_STATIC_CRT=OFF -DBUILD_opencv_apps=OFF -DWITH_1394=OFF -DWITH_MATLAB=OFF -DWITH_WEBP=OFF
			msbuild OpenCV.sln /m /nologo /p:Configuration=Debug /p:Platform=Win32
			msbuild OpenCV.sln /m /nologo /p:Configuration=Release /p:Platform=Win32
		)
	)
	echo Return to %currentLoc%
	cd %currentLoc%
) else (
	rem Если нету переменной среды OpenCV_DIR, то проставить
	echo OpenCV_DIR environment not exists, auto setup
	echo Go to PROJECT_DIR
	cd %currentLoc%
	echo !cd!
	cd ../..
	echo Current dir is: !cd!
	git submodule update --init -- "ThirdParty/OpenCV"
	
	echo Go to OpenCV Directory
	cd ThirdParty\OpenCV
	echo !cd!
	echo !cd!
	echo !cd!
	
	setx OpenCV_DIR !cd!
	set OpenCV_DIR=!cd!
	
	echo OpenCV_DIR is now: !OpenCV_DIR!
	
	if "%1" == "x64" (
		if not exist "!OpenCV_DIR!\Win64\" (
			mkdir Win64 && cd Win64
			echo Building OpenCV for Win64 - !OpenCV_DIR!\Win64
			cmake .. -G"Visual Studio 15 2017 Win64" -DBUILD_JASPER=OFF -DBUILD_JAVA=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_WEBP=OFF -DBUILD_WITH_STATIC_CRT=OFF -DBUILD_opencv_apps=OFF -DWITH_1394=OFF -DWITH_MATLAB=OFF -DWITH_WEBP=OFF
			msbuild OpenCV.sln /m /nologo /p:Configuration=Debug /p:Platform=x64
			msbuild OpenCV.sln /m /nologo /p:Configuration=Release /p:Platform=x64
		)
	) else (
		if not exist "!OpenCV_DIR!\Win\" (
			mkdir Win && cd Win
			echo Building OpenCV for Win32 - !OpenCV_DIR!\Win
			cmake .. -G"Visual Studio 15 2017" -DBUILD_JASPER=OFF -DBUILD_JAVA=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_WEBP=OFF -DBUILD_WITH_STATIC_CRT=OFF -DBUILD_opencv_apps=OFF -DWITH_1394=OFF -DWITH_MATLAB=OFF -DWITH_WEBP=OFF
			msbuild OpenCV.sln /m /nologo /p:Configuration=Debug /p:Platform=Win32
			msbuild OpenCV.sln /m /nologo /p:Configuration=Release /p:Platform=Win32
		)
	)
)

echo Go to '%currentLoc%'
cd %currentLoc%

