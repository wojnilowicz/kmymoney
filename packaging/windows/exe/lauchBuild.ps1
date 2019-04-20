Write-Host $Env:Path
$Env:Path="C:\msys64\usr\bin;C:\Program Files\CMake\bin;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;$Env:Path"
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
bash -lc "pacman -Ss git"
bash -lc "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja"
# bash /$buildUnix/packaging/windows/exe/build.sh deps /$homeUnix/workspace /$buildUnix

