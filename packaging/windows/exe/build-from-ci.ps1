$TIMER = [System.Diagnostics.Stopwatch]::StartNew()
if ($args.count -ne 3) {
  Write-Host "3 arguments required to go further. Exiting."
  exit
}

$CUTOFF_TIME = $args[0]
$WORKSPACE_PATH = "/" + (($args[1] -replace "\\","/") -replace ":","").ToLower().TrimEnd("/").TrimStart("/")
$KMYMONEY_SOURCES = "/" + (($args[2] -replace "\\","/") -replace ":","").ToLower().TrimEnd("/").TrimStart("/")

if ($Env:TRAVIS) {

  cinst -y --no-progress msys2 --params="'/NoUpdate /InstallDir=C:\msys64'"

  $Env:PATH = (
    "C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin",
    "C:\msys64\mingw64\bin",
    "C:\msys64\usr\bin",
    "C:\Python37\Scripts",
    "C:\Python37",
    "C:\Program Files\CMake\bin",
    "C:\Program Files\Git\cmd",
    "C:\Program Files (x86)\NSIS",
    "C:\ProgramData\chocolatey\bin"
  ) -join ";"

  $Env:UPLOADTOOL_SUFFIX="windows-travis"

  bash -c "pacman -S --needed --noconfirm make patch tar mingw-w64-x86_64-ninja mingw-w64-x86_64-pkg-config bison perl"

} elseif ($Env:APPVEYOR) {

  $Env:PATH = (
    "C:\msys64\mingw64\bin",
    "C:\msys64\usr\bin",
    "C:\Python37-x64\Scripts",
    "C:\Python37-x64",
    "C:\Program Files (x86)\CMake\bin",
    "C:\Program Files\Git\cmd",
    "C:\Program Files (x86)\NSIS",
    "C:\ProgramData\chocolatey\bin"
  ) -join ";"

  $Env:UPLOADTOOL_SUFFIX = "windows-appveyor"

  bash -c "pacman -S --needed --noconfirm mingw-w64-x86_64-ninja"
  bash -c "pip3 install meson"
}

foreach ($BUILD_STAGE in @("deps", "kmymoney", "image")) {
  $REMAINING_TIME = ($CUTOFF_TIME - [math]::Round($TIMER.Elapsed.TotalMinutes))

  $TIMEDOUT_FILENAME="TIMEDOUT"
  Remove-Item $TIMEDOUT_FILENAME -ErrorAction SilentlyContinue

  if (($BUILD_STAGE -eq "deps" -and $REMAINING_TIME -le 0) -or
      ($BUILD_STAGE -eq "kmymoney" -and $REMAINING_TIME -lt 2) -or
      ($BUILD_STAGE -eq "image" -and $REMAINING_TIME -lt 2)) {
    Write-Host "No time for building '${BUILD_STAGE}'."
    bash -c "touch ${TIMEDOUT_FILENAME}"
    break
  }

  $COMMAND = (
    "timeout ${REMAINING_TIME}m",
    "${KMYMONEY_SOURCES}/packaging/windows/exe/build.sh",
    "${BUILD_STAGE}",
    "${WORKSPACE_PATH}",
    "${KMYMONEY_SOURCES}",
    "|| touch ${TIMEDOUT_FILENAME}"
  ) -join " "

  if ($BUILD_STAGE -eq "image") {
    cinst -y --no-progress nsis
  }

  bash -c $COMMAND
  if (Test-Path $TIMEDOUT_FILENAME -PathType Leaf) {
    Write-Host "Building '${BUILD_STAGE}' did not finish."
    break
  }
}

if ($BUILD_STAGE -eq "image" -and -not (Test-Path $TIMEDOUT_FILENAME -PathType Leaf)) {
  if ($Env:TRAVIS) {
#     bash -c "wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh"
#     bash -c "bash upload.sh ${WORKSPACE_PATH}/image-build/*.exe"
    bash -c "bash ${KMYMONEY_SOURCES}/packaging/common/upload.sh ${WORKSPACE_PATH}/image-build/*.exe"
  } elseif ($Env:APPVEYOR) {
    bash -c "bash ${KMYMONEY_SOURCES}/packaging/common/upload.sh ${WORKSPACE_PATH}/image-build/*.exe"
  }
} else {
  Write-Host "Image will not be sent."
}

if ($Env:TRAVIS -or $Env:APPVEYOR) {
  Write-Host "Stoping all build processes."
  Stop-Process -Name make -ErrorAction SilentlyContinue
  Stop-Process -Name ninja -ErrorAction SilentlyContinue
  $cmake = Get-Process cmake -ErrorAction SilentlyContinue
  if ($cmake) {
    Write-Host "Waiting for cmake to stop."
    $cmake | Wait-Process -Timeout 60 # CMake might be unpacking, so wait for it to avoid caching issues
    Write-Host "Stoping cmake."
    Stop-Process -Name cmake -ErrorAction SilentlyContinue
  }
}