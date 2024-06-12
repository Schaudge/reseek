#!/bin/bash

test -z $(git status --porcelain) 2> /dev/null
if [ $? != 0 ] ; then
	echo
	echo "ERROR -- uncommitted changes"
	echo
	exit 1
fi

p='/c/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.40.33807/bin/HostX86/x86:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/VC/VCPackages:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer:/c/Program Files/Microsoft Visual Studio/2022/Community/MSBuild/Current/bin/Roslyn:/c/Program Files/Microsoft Visual Studio/2022/Community/Team Tools/DiagnosticsHub/Collector:/c/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0//x86:/c/Program Files (x86)/Windows Kits/10/bin//x86:/c/Program Files/Microsoft Visual Studio/2022/Community//MSBuild/Current/Bin/amd64:/c/Windows/Microsoft.NET/Framework/v4.0.30319:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/:/c/cygwin64/usr/local/bin:/c/cygwin64/bin:/c/Windows/system32:/c/Windows:/c/Windows/System32/Wbem:/c/Windows/System32/WindowsPowerShell/v1.0:/c/Windows/System32/OpenSSH:/c/Program Files/dotnet:/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/c/Program Files (x86)/GitHub CLI:/c/Program Files/CMake/bin:/c/Users/bob/AppData/Local/Microsoft/WindowsApps:/c/Users/bob/AppData/Local/Google/Cloud SDK/google-cloud-sdk/bin:/c/src/dss/Release:/c/src/reseek/src/Release:/c/src/palmscan2/Release:/c/src/muscle/src/x64/Release:/c/a/scripts:/c/src/py:/c/exe:/c/Program Files (x86)/GitHub CLI/:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja:/c/Program Files/Microsoft Visual Studio/2022/Community/Common7/IDE/VC/Linux/bin/ConnectionManagerExe:/c/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg'
export PATH=$PATH:$p

echo \"$(git log --oneline | head -n1 | cut "-d " -f1)\" | tee gitver.txt

MSBuild.exe -property:Configuration=Release | tee MSBuild.log
