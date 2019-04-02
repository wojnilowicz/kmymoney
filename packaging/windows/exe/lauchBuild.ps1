$homeUnix= "/"(($Env:HOME -replace "\\","/") -replace ":","").ToLower().Trim("/")
$buildUnix= "/"(($Env:TRAVIS_BUILD_DIR -replace "\\","/") -replace ":","").ToLower().Trim("/")
bash $buildUnix/packaging/windows/exe/build.sh deps $homeUnix/workspace $buildUnix
