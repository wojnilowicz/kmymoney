Write-Host $Env:Path
# $Env:Path='C:\tools\msys64\usr\bin;C:\Program Files\CMake\bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0;C:\Windows\System32\OpenSSH;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin'

$Env:Path='C:\tools\msys64\usr\bin;' + $Env:Path

bash --version
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
bash -lc "pacman -S make"
bash /$buildUnix/packaging/windows/exe/build.sh deps /$homeUnix/workspace /$buildUnix

