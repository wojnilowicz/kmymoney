Write-Host $Env:Path
# $Env:Path="C:\Python37\Script;C:\Python37\bin;C:\msys64\usr\bin;C:\Program Files\CMake\bin;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;$Env:Path"
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
if ( $args[0]="pacman-deps" ) {
  bash -lc "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja"
} elseif ($args[0]="update-msys2") {
  bash -lc "pacman -Syu"
} else {
  bash /$buildUnix/packaging/windows/exe/build.sh $args[0] /c /$buildUnix
}



