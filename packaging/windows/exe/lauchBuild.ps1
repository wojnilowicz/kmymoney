Write-Host $Env:Path
$Env:Path="C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin;C:\msys64\usr\bin;C:\msys64\mingw64\bin;C:\Python37\Scripts;C:\Python37;C:\Program Files\CMake\bin;C:\Program Files\Git\cmd;C:\ProgramData\chocolatey\bin"
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
  Write-Host "In pacman-deps"
  bash -c "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja mingw-w64-x86_64-pkg-config"
} elseif ($args[0] -eq "update-msys2") {
  bash -c "pacman -Syu --noconfirm"
  bash -c "pacman -Su --noconfirm"

} else {
  bash -c "timeout $($args[1])m /$buildUnix/packaging/windows/exe/build.sh $($args[0]) /c /$buildUnix || true"
  Stop-Process -Name make -ErrorAction SilentlyContinue
  Stop-Process -Name ninja -ErrorAction SilentlyContinue
  Stop-Process -Name sh -ErrorAction SilentlyContinue
  Wait-Process -Timeout 60 -Name cmake # CMake might be unpacking, so wait for it to avoid caching issues
  Stop-Process -Name cmake -ErrorAction SilentlyContinue
}
