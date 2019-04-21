Write-Host $Env:Path
$Env:Path="C:\Python37\Scripts;C:\Python37\bin;C:\msys64\usr\bin;C:\Program Files\CMake\bin;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;C:\Program Files\Git\cmd"
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
if ( $args[0] -eq "pacman-deps") {
  bash -lc "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja perl"
} elseif ($args[0] -eq "update-msys2") {
  bash -lc "pacman -Syu --noconfirm"
  bash -lc "pacman -Su --noconfirm"

} else {
  bash /$buildUnix/packaging/windows/exe/build.sh $args[0] /c /$buildUnix
}



