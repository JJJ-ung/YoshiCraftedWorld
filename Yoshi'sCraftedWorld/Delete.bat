rmdir /s /q "ipch"
rmdir /s /q "Debug"
rmdir /s /q "Release"

rmdir /s /q .\Client\Default\Debug
rmdir /s /q .\Client\Default\ipch
rmdir /s /q .\Client\Default\Release
rmdir /s /q .\Client\Default\x64

rmdir /s /q .\Engine\Default\Debug
rmdir /s /q .\Engine\Default\Release

del /f /q .\Client\Bin\Debug\Client.exe
del /f /q .\Client\Bin\Debug\Client.ilk
del /f /q .\Client\Bin\Debug\Client.pdb
del /f /q YoshiCraftedWorld.VC.db
