@ECHO OFF

cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tcshellcode_runner.cpp /link /OUT:Shellcode_Runner.exe /SUBSYSTEM:CONSOLE /MACHINE:x64