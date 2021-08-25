; The name of the installer
Name "AnyMeal"

; The file to write
OutFile "anymeal-installer-1.14.exe"

; The default installation directory
InstallDir $PROGRAMFILES\AnyMeal

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\NSIS_AnyMeal" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "AnyMeal (required)"

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there
  File ".libs\anymeal.exe"
  File "anymeal.ico"
  File "..\LICENSE"
  File "C:\msys64\mingw64\bin\libbrotlicommon.dll"
  File "C:\msys64\mingw64\bin\libbrotlidec.dll"
  File "C:\msys64\mingw64\bin\libbz2-1.dll"
  File "C:\msys64\mingw64\bin\libdouble-conversion.dll"
  File "C:\msys64\mingw64\bin\libfreetype-6.dll"
  File "C:\msys64\mingw64\bin\libgcc_s_seh-1.dll"
  File "C:\msys64\mingw64\bin\libglib-2.0-0.dll"
  File "C:\msys64\mingw64\bin\libgraphite2.dll"
  File "C:\msys64\mingw64\bin\libharfbuzz-0.dll"
  File "C:\msys64\mingw64\bin\libiconv-2.dll"
  File "C:\msys64\mingw64\bin\libicudt67.dll"
  File "C:\msys64\mingw64\bin\libicuin67.dll"
  File "C:\msys64\mingw64\bin\libicuuc67.dll"
  File "C:\msys64\mingw64\bin\libintl-8.dll"
  File "C:\msys64\mingw64\bin\libpcre-1.dll"
  File "C:\msys64\mingw64\bin\libpcre2-16-0.dll"
  File "C:\msys64\mingw64\bin\libpng16-16.dll"
  File "C:\msys64\mingw64\bin\librecode-3.dll"
  File "C:\msys64\mingw64\bin\libsqlite3-0.dll"
  File "C:\msys64\mingw64\bin\libstdc++-6.dll"
  File "C:\msys64\mingw64\bin\libwinpthread-1.dll"
  File "C:\msys64\mingw64\bin\libzstd.dll"
  File "C:\msys64\mingw64\bin\Qt5Core.dll"
  File "C:\msys64\mingw64\bin\Qt5Gui.dll"
  File "C:\msys64\mingw64\bin\Qt5Svg.dll"
  File "C:\msys64\mingw64\bin\Qt5PrintSupport.dll"
  File "C:\msys64\mingw64\bin\Qt5Widgets.dll"
  File "C:\msys64\mingw64\bin\zlib1.dll"

  SetOutPath "$INSTDIR\platforms"
  File "C:\msys64\mingw64\share\qt5\plugins\platforms\qwindows.dll"

  SetOutPath "$INSTDIR\imageformats"
  File "C:\msys64\mingw64\share\qt5\plugins\imageformats\qsvg.dll"

  SetOutPath "$INSTDIR\printsupport"
  File "C:\msys64\mingw64\share\qt5\plugins\printsupport\windowsprintersupport.dll"

  SetOutPath "$INSTDIR\styles"
  File "C:\msys64\mingw64\share\qt5\plugins\styles\qwindowsvistastyle.dll"

  SetOutPath "$INSTDIR\locale\de"
  File "locale\de\anymeal_qt.qm"
  SetOutPath "$INSTDIR\locale\nl"
  File "locale\nl\anymeal_qt.qm"
  SetOutPath "$INSTDIR\locale\it"
  File "locale\it\anymeal_qt.qm"

  SetOutPath $INSTDIR

  ; Write the installation path into the registry
  WriteRegStr HKLM "SOFTWARE\NSIS_AnyMeal" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "DisplayName" "AnyMeal"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "DisplayIcon" '"$INSTDIR\anymeal.ico"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "Publisher" "Jan Wedekind"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "DisplayVersion" "1.14"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "EstimatedSize" 76022
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\AnyMeal"
  CreateShortCut "$SMPROGRAMS\AnyMeal\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\AnyMeal\AnyMeal.lnk" "$INSTDIR\anymeal.exe" "" "$INSTDIR\anymeal.exe" 0

SectionEnd

Section "Desktop Shortcut"
  CreateShortCut "$DESKTOP\AnyMeal.lnk" "$INSTDIR\anymeal.exe" "" "$INSTDIR\anymeal.exe" 0
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\AnyMeal"
  DeleteRegKey HKLM "SOFTWARE\NSIS_AnyMeal"

  ; Remove files and uninstaller
  Delete "$INSTDIR\styles\*.dll"
  Delete "$INSTDIR\printsupport\*.dll"
  Delete "$INSTDIR\imageformats\*.dll"
  Delete "$INSTDIR\platforms\*.dll"
  Delete "$INSTDIR\locale\it\*.qm"
  Delete "$INSTDIR\locale\nl\*.qm"
  Delete "$INSTDIR\locale\de\*.qm"
  Delete "$INSTDIR\*.dll"
  Delete $INSTDIR\LICENSE
  Delete $INSTDIR\anymeal.exe
  Delete $INSTDIR\anymeal.ico
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\AnyMeal\*.*"
  Delete "$DESKTOP\AnyMeal.lnk"

  ; Remove directories used
  RMDir "$SMPROGRAMS\AnyMeal"
  RMDir "$INSTDIR\locale\it"
  RMDir "$INSTDIR\locale\nl"
  RMDir "$INSTDIR\locale\de"
  RMDir "$INSTDIR\locale"
  RMDir "$INSTDIR\styles"
  RMDir "$INSTDIR\printsupport"
  RMDir "$INSTDIR\imageformats"
  RMDir "$INSTDIR\platforms"
  RMDir "$INSTDIR"

SectionEnd
