@echo off

REM ��ȡ��ǰĿ¼�µ��ļ�������
for /d %%i in (*) do (
    echo ���ڴ����ļ���: %%i
    JackAnalyzer.exe .\%%i\
    for %%j in (.\%%i\*.xml) do (
        echo ���ڴ����ļ�: %%j
        ..\..\tools\TextComparer.bat %%j .\%%i\out\%%~nj_output.xml 
        pause
    )    
)
