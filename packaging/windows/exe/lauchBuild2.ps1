Write-Host $Env:Path
$Env:Path="C:\Program Files\NSIS;C:\Program Files (x86)\NSIS;C:\Python37-x64;C:\Python37-x64\Scripts;C:\msys64\mingw64\bin;C:\msys64\usr\bin;C:\Program Files (x86)\CMake\bin;C:\Program Files\Git\cmd"

$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:APPVEYOR_BUILD_FOLDER -replace "\\","/") -replace ":","").ToLower().Trim("/")
# $args[0] could be:
# 1) pacman-deps
# 2) update-msys2
# 3) deps
# 4) kmymoney
# 5) image

# $args[1] is timeout for build script

if ( $args[0] -eq "pacman-deps") {
  Write-Host "In pacman-deps"
  bash -c "pacman -S --needed --noconfirm mingw-w64-x86_64-ninja"  #mingw-w64-x86_64-python3-pip
} elseif ($args[0] -eq "update-msys2") {
  bash -c "pacman -Syu --noconfirm"
  bash -c "pacman -Su --noconfirm"

} elseif ($args[0] -eq "upload-image") {
  bash -c "wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh"
  bash -c "export UPLOADTOOL_SUFFIX=appveyor"
  bash -c "upload.sh /c/image-build/*.exe"

} else {
  bash -c "timeout $($args[1])m /$buildUnix/packaging/windows/exe/build.sh $($args[0]) /c /$buildUnix"
  Stop-Process -Name make -ErrorAction SilentlyContinue
  Stop-Process -Name ninja -ErrorAction SilentlyContinue
  Stop-Process -Name cmake -ErrorAction SilentlyContinue
  Stop-Process -Name sh -ErrorAction SilentlyContinue
}
