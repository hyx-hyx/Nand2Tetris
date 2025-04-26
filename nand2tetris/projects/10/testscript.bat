@echo off

REM 获取当前目录下的文件夹名称
for /d %%i in (*) do (
    echo 正在处理文件夹: %%i
    JackAnalyzer.exe .\%%i\
    for %%j in (.\%%i\*.xml) do (
        echo 正在处理文件: %%j
        ..\..\tools\TextComparer.bat %%j .\%%i\out\%%~nj_output.xml 
        pause
    )    
)
