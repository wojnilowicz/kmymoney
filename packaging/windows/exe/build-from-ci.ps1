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

  $Env:Path = (
    "C:\ProgramData\chocolatey\lib\mingw\tools\install\mingw64\bin",
    "C:\msys64\mingw64\bin",
    "C:\Python37\Scripts",
    "C:\Python37",
    "C:\Program Files\CMake\bin",
    "C:\Program Files\Git\cmd",
    "C:\Program Files (x86)\NSIS",
    "C:\ProgramData\chocolatey\bin"
  ) -join ";"

  $Env:UPLOADTOOL_SUFFIX="windows-travis"

  bash -c "pacman -S --needed --noconfirm make patch mingw-w64-x86_64-ninja mingw-w64-x86_64-pkg-config bison"

} elseif ($Env:APPVEYOR) {

  $Env:Path = (
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
}

foreach ($BUILD_STAGE in @("kmymoney", "image")) {
  $REMAINING_TIME = ($CUTOFF_TIME - [math]::Round($TIMER.Elapsed.TotalMinutes))
  if ($REMAINING_TIME -gt 0) {

      $COMMAND = (
        "timeout ${REMAINING_TIME}m",
        "${KMYMONEY_SOURCES}/packaging/windows/exe/build.sh",
        "${BUILD_STAGE}",
        "${WORKSPACE_PATH}",
        "${KMYMONEY_SOURCES}",
        "|| echo 'unfinished'"
      ) -join " "

    if ($BUILD_STAGE -eq "image") {
      cinst -y --no-progress nsis
    }

    $STATUS = (bash -c $COMMAND)
    if ($STATUS -eq "unfinished") {
      Write-Host "Building '${BUILD_STAGE}' did not finish."
      break
    }

  } else {
    break
  }
  break
}

if ($BUILD_STAGE -eq "image" -and -not $STATUS) {
  if ($Env:TRAVIS) {
    bash -c "wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh"
    bash -c "bash upload.sh ${WORKSPACE_PATH}/image-build/*.exe"
  } elseif ($Env:APPVEYOR) {
    bash -c "bash ${KMYMONEY_SOURCES}/packaging/windows/exe/upload.sh ${WORKSPACE_PATH}/image-build/*.exe"
  }
}

if ($Env:TRAVIS) {
  Stop-Process -Name make -ErrorAction SilentlyContinue
  Stop-Process -Name ninja -ErrorAction SilentlyContinue
  $cmake = Get-Process cmake -ErrorAction SilentlyContinue
  if ($cmake) {
    $cmake | Wait-Process -Timeout 60 # CMake might be unpacking, so wait for it to avoid caching issues
    if (!$cmake.HasExited) {
      $cmake | Stop-Process -Force
    }
  }
}