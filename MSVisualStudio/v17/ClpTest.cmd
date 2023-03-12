@REM This file will run command line tests, comparable to test\makefile.am

@echo off
SET "BINDIR=%~1"
SET "SAMPLEDIR=%~2"

echo INFO: Running %0 %*
echo INFO: Using bindir '%BINDIR%' and sampledir '%SAMPLEDIR%'

if "%BINDIR%"=="" goto :usage
if not exist "%BINDIR%" echo ERROR: Folder %BINDIR% does not exist. && goto :usage
if "%SAMPLEDIR%"=="" goto :usage
if not exist %SAMPLEDIR% echo ERROR: Folder %SAMPLEDIR% does not exist. && goto :usage
if not %errorlevel%==0 echo ERROR: %SAMPLEDIR% cannot contain spaces. && goto :usage

goto :test

:usage
echo INFO: Usage %0 ^<bindir^> ^<sampledir^>
echo INFO: where ^<bindir^> contains the executables and sampledir the sample files.
echo INFO: The ^<sampledir^> must not contain spaces!
echo INFO: For example: %0 "D:\Some Directory\" "..\..\samples\"
goto :error

:error
echo ERROR: An error occurred while running the tests!
echo INFO: Finished Tests but failed
exit /b 1

:test
echo INFO: Starting Tests

"%BINDIR%\clp.exe" -dirSample %SAMPLEDIR% -unitTest
if not %errorlevel%==0 goto :error

if exist "%BINDIR%\OsiClpUnitTest.exe" (
  "%BINDIR%\OsiClpUnitTest.exe" -mpsDir=%SAMPLEDIR%
  if not %errorlevel%==0 goto :error
)

echo INFO: Finished Tests successfully (%ERRORLEVEL%)