Write-Host $Env:Path
Write-Host $args[0]
$Env:Path="C:\msys64\usr\bin;C:\Program Files\CMake\bin;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;$Env:Path"
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
if ( $args[0]="pacman-deps" ) {
  Write-Host "In Pacman deps"
  bash -lc "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja"
} else {
  bash /$buildUnix/packaging/windows/exe/build.sh $args[0] /c /$buildUnix
}



