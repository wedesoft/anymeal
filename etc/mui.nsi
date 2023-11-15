!include "MUI2.nsh"

!define APPNAME "Anymeal"
!define COMPANYNAME "Wedesoft"
!define EXENAME "anymeal.exe"
!define EXEARGS ""
!define SOURCEDIR "anymeal"
!define INSTALLERNAME "anymeal-installer.exe"
!define COMPONENTNAME "Anymeal"

!define MUI_ICON "logo.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "logo.bmp"
!define MUI_HEADERIMAGE_RIGHT

!define MUI_WELCOMEPAGE_TITLE "Anymeal"

!define MUI_WELCOMEFINISHPAGE_BITMAP "endimage.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "endimage.bmp"

!define MUI_COMPONENTSPAGE_NODESC

!define MUI_FINISHPAGE_RUN "$INSTDIR\${EXENAME}"
!define MUI_FINISHPAGE_RUN_TEXT "Launch ${APPNAME}"
!define MUI_FINISHPAGE_RUN_PARAMETERS "${EXEARGS}"

Name "${APPNAME}"

SetCompressor lzma
SetCompress force

OutFile "..\artifacts\${INSTALLERNAME}"
InstallDir "$PROGRAMFILES64\${COMPANYNAME}\${APPNAME}"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Function .onInit
  System::Call 'kernel32::CreateMutex(p 0, i 0, t "anymealInstaller") p .r1 ?e'
  Pop $R0

  StrCmp $R0 0 +3
    MessageBox MB_OK|MB_ICONEXCLAMATION "The installer is already running. Please close all other instances and try again."
    Abort
FunctionEnd

Section "${COMPONENTNAME}"
  # This section is not optional
  SectionIn RO

  # Remove previous installation
  RmDir /r "$INSTDIR\*.*"
  RmDir "$INSTDIR"

  # Write files
  setOutPath $INSTDIR
  file /r "${SOURCEDIR}\*.*"

  # Create uninstaller
  writeUninstaller "$INSTDIR\uninstall.exe"

  # Register uninstaller
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayName" "${APPNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "DisplayIcon" '"$INSTDIR\logo.ico"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "Publisher" "${COMPANYNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}" "NoRepair" 1
SectionEnd

Section "Start Menu Shortcuts"
  # Create Start Menu folder and shortcut
  CreateDirectory "$SMPROGRAMS\${COMPANYNAME}"
  CreateShortcut "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}.lnk" "$INSTDIR\${EXENAME}" "${EXEARGS}" "$INSTDIR\logo.ico" 0
SectionEnd

Section "Create Desktop Shortcuts"
  CreateShortcut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\${EXENAME}" "${EXEARGS}" "$INSTDIR\logo.ico" 0
SectionEnd

Section "uninstall"
  # Remove Start Menu shortcut and attempt to remove the folder
  Delete "$SMPROGRAMS\${COMPANYNAME}\${APPNAME}.lnk"
  RmDir "$SMPROGRAMS\${COMPANYNAME}"

  # Remove uninstaller information from registry
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${APPNAME}"

  # Remove files and folder
  RmDir /r "$INSTDIR\*.*"
  RmDir "$INSTDIR"
SectionEnd
