;Copyright 2019 Łukasz Wojniłowicz <lukasz.wojnilowicz@gmail.com>

;Based on Basic Example Script written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General
  Name "@{appName}"     # appears on title bar
  OutFile "@{outFile}"  # filename of installer's executable

  !define INSTALL_REG_KEY "Software\@{company}\@{appName}"
  !define UNINSTALL_REG_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\@{appName}"
  !define MUI_ICON "@{iconFile}"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\nsis3-metro.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\nsis3-metro.bmp"
  SetCompressor /SOLID lzma
  SetCompress force

;--------------------------------
;Setting up possible admin installation

  !define MULTIUSER_EXECUTIONLEVEL Highest            # request admin permission if possible
  !define MULTIUSER_MUI                               # needed for multi user page
  !define MULTIUSER_INSTALLMODE_COMMANDLINE           # needed for multi user silent installs
  !define MULTIUSER_INSTALLMODE_INSTDIR "@{appName}"  # without this no INSTDIR is set
  !define MULTIUSER_USE_PROGRAMFILES64                # install to ProgramFiles64 and not ProgramFiles
  !include "MultiUser.nsh"                            # must appear as the last row

  Function .onInit
    !insertmacro MULTIUSER_INIT # initializes multi user on installation
  FunctionEnd

  Function un.onInit
    !insertmacro MULTIUSER_UNINIT # initializes multi user on uninstallation
  FunctionEnd

;--------------------------------
;Setting up start menu directory

  Var StartMenuFolder
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "SHCTX"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "${INSTALL_REG_KEY}"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING # shows warning when trying to quit installer

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "@{licenseFile}"
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MULTIUSER_PAGE_INSTALLMODE
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Core" coreComponentID
  SectionIn RO

  ;Store installation directory
  WriteRegStr SHCTX "${INSTALL_REG_KEY}" "Install_Dir" $INSTDIR

  ;Define some unistaller meta data
  WriteRegStr SHCTX "${UNINSTALL_REG_KEY}" "DisplayName" "@{appName}"
  WriteRegStr SHCTX "${UNINSTALL_REG_KEY}" "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr SHCTX "${UNINSTALL_REG_KEY}" "DisplayIcon" "$INSTDIR\@{iconName}"
  WriteRegStr SHCTX "${UNINSTALL_REG_KEY}" "URLInfoAbout" "@{website}"
  WriteRegStr SHCTX "${UNINSTALL_REG_KEY}" "Publisher" "@{publisher}"
  WriteRegStr SHCTX "${UNINSTALL_REG_KEY}" "DisplayVersion" "@{version}"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortCut "$SMPROGRAMS\$StartMenuFolder\KMyMoneyNEXT.lnk" "$INSTDIR\bin\kmymoney.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

  SetOutPath "$INSTDIR"
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  ;Add files to be packaged in installer
  File /a "@{iconFile}"
  SetOutPath "$INSTDIR"
  File /r "bin"

SectionEnd

SectionGroup "Plugins" pluginComponentID
  Section "QIF" qifComponentID
    SetOutPath "$INSTDIR\bin\kmymoney"
    File /r "qif\plugin\*"

    SetOutPath "$INSTDIR\bin\data\kservices5"
    File /r "qif\service\*"
  SectionEnd

  Section "OFX" ofxComponentID
    SetOutPath "$INSTDIR\bin\kmymoney"
    File /r "ofx\plugin\*"

    SetOutPath "$INSTDIR\bin"
    File /r "ofx\libs\*"
  SectionEnd

  Section "Online banking" onlineBankingComponentID
    SetOutPath "$INSTDIR\bin\kmymoney"
    File /r "onlinebanking\plugin\*"

    SetOutPath "$INSTDIR\bin"
    File /r "onlinebanking\libs\*"

    SetOutPath "$INSTDIR\bin\data"
    File /r "onlinebanking\data\*"
  SectionEnd
SectionGroupEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString coreComponentDescription ${LANG_ENGLISH} "Basic functionality of KMyMoneyNEXT."
  LangString pluginComponentDescription ${LANG_ENGLISH} "Extended functionality of KMyMoneyNEXT."
  LangString qifComponentDescription ${LANG_ENGLISH} "QIF importing and exporting support."
  LangString ofxComponentDescription ${LANG_ENGLISH} "OFX importing support."
  LangString onlineBankingComponentDescription ${LANG_ENGLISH} "Sending and receiving personal financial data from the internet."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${coreComponentID} $(coreComponentDescription)
    !insertmacro MUI_DESCRIPTION_TEXT ${pluginComponentID} $(pluginComponentDescription)
    !insertmacro MUI_DESCRIPTION_TEXT ${qifComponentID} $(qifComponentDescription)
    !insertmacro MUI_DESCRIPTION_TEXT ${ofxComponentID} $(ofxComponentDescription)
    !insertmacro MUI_DESCRIPTION_TEXT ${onlineBankingComponentID} $(onlineBankingComponentDescription)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"
  RMDir /r "$INSTDIR"

  ;If admin removes then remove from HKLM as well
  ${If} $MultiUser.InstallMode == "AllUsers"
    SetShellVarContext all  ; set SHCTX to HKLM
    DeleteRegKey SHCTX "${UNINSTALL_REG_KEY}"
    DeleteRegKey SHCTX "${INSTALL_REG_KEY}"
    !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    RMDir /r "$SMPROGRAMS\$StartMenuFolder"
  ${EndIf}

  SetShellVarContext current ; set SHCTX to HKCU
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  RMDir /r "$SMPROGRAMS\$StartMenuFolder"
  DeleteRegKey SHCTX "${UNINSTALL_REG_KEY}"
  DeleteRegKey SHCTX "${INSTALL_REG_KEY}"

SectionEnd

;--------------------------------
;Version Information

VIProductVersion "1.0.0.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "@{appName}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "Comments" "Personal Finance Manager"
;VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Fake company"
;VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalTrademarks" "Test Application is a trademark of Fake company"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright Lukasz Wojnilowicz"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "KMyMoneyNEXT installer"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "@{version}"


