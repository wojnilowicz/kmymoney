Write-Host $Env:Path
$Env:Path="C:\msys64\mingw64\bin;C:\Python37\Scripts;C:\Python37;C:\msys64\usr\bin;C:\Program Files\CMake\bin;C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;C:\Program Files\Git\cmd"
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
# $args[0] could be:
# 1) pacman-deps
# 2) update-msys2
# 3) deps
# 4) kmymoney
# 5) image

# $args[1] is timeout for build script

if ( $args[0] -eq "pacman-deps") {
  bash -lc "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja perl"
} elseif ($args[0] -eq "update-msys2") {
  bash -lc "pacman -Syu --noconfirm"
  bash -lc "pacman -Su --noconfirm"

} else {
#   bash -lc "/$buildUnix/packaging/windows/exe/build.sh " + $args[0] " /c /$buildUnix " + $args[1]
  bash -lc "/$buildUnix/packaging/windows/exe/build.sh deps /c /$buildUnix 5"
}



