Write-Host $Env:Path
$Env:Path='C:\tools\msys64\usr\bin;' + $Env:Path
C:\tools\msys64\usr\bin\bash.exe --version
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
C:\tools\msys64\usr\bin\bash.exe -lc "pacman -Ss git"
C:\tools\msys64\usr\bin\bash.exe /$buildUnix/packaging/windows/exe/build.sh deps /$homeUnix/workspace /$buildUnix
bash --version

