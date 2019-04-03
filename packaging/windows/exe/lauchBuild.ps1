Write-Host $Env:Path
$Env:Path='C:\tools\msys64\usr\bin;' + $Env:Path
bash --version
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
bash -lc "pacman -S --needed --noconfirm make mingw-w64-x86_64-zlib mingw-w64-x86_64-pkg-config"
bash /$buildUnix/packaging/windows/exe/build.sh deps /$homeUnix/workspace /$buildUnix

