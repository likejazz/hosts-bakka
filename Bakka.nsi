;--------------------------------
;Define Settings

  !define VERSION	1.4.0


;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------
;General
  
  ;Name and file
  Name "Bakka"
  OutFile "Bakka_Setup_${VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Bakka"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Bakka" ""
  
  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin
  
  BrandingText "Bakka ${VERSION}"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Orange Theme Settings

; MUI Settings / Icons
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\orange-uninstall.ico"
 
; MUI Settings / Header
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_RIGHT
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-r.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\orange-uninstall-r.bmp"
 
; MUI Settings / Wizard
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange-uninstall.bmp"


;--------------------------------
;Pages

  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
  !define MUI_FINISHPAGE_RUN "$INSTDIR\Bakka.exe"
  !define MUI_FINISHPAGE_RUN_TEXT "Run Bakka"
  
  !insertmacro MUI_PAGE_FINISH  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Bakka" SecDummy

  SetOutPath "$INSTDIR"
  
  File /a "Bakka.exe"
  File /a "Bakka.ini"
  
  CreateDirectory "$SMPROGRAMS\Bakka"
  CreateShortCut "$SMPROGRAMS\Bakka\Bakka.lnk" "$INSTDIR\Bakka.exe"
  
  ;Store installation folder
  WriteRegStr HKCU "Software\Bakka" "" $INSTDIR
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka" \
	"DisplayName" "Bakka (remove only)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka" \
	"UninstallString" '"$INSTDIR\Uninstall.exe"'
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\Bakka.exe"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$SMPROGRAMS\Bakka\Bakka.lnk"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Bakka"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Bakka"

SectionEnd