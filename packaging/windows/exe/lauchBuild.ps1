bash --version
$homeUnix= (($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= (($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
bash -lc "pacman -S --needed --noconfirm git"
bash /$buildUnix/packaging/windows/exe/build.sh deps /$homeUnix/workspace /$buildUnix

