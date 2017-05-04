@ECHO OFF
copy .\_alias.bat %USERPROFILE%\
REG DELETE "HKCU\SOFTWARE\Microsoft\Command Processor" /v AutoRun /f
REG ADD "HKCU\SOFTWARE\Microsoft\Command Processor" /v AutoRun /t REG_SZ /d %USERPROFILE%\_alias.bat
ECHO "Setup succeeded."