!define PRODUCT_NAME "autonice"
!define PRODUCT_VERSION "1.2.0"
!define PRODUCT_PUBLISHER "markdevel"
!define PRODUCT_WEB_SITE "http://markdevel.hatenablog.com/"
!define PRODUCT_DIR_REGKEY "Software\markdevel\autonice"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor /SOLID /FINAL LZMA

!include "MUI2.nsh"
!include "x64.nsh"
!include "LogicLib.nsh"
!include "Sections.nsh"

!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\win-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\win-uninstall.ico"
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\autongui.exe"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "Japanese"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "setup-${PRODUCT_NAME}-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES64\${PRODUCT_NAME}"
ShowInstDetails show
ShowUnInstDetails show

Section
  SetShellVarContext all
  SetOutPath $INSTDIR
  SetOverwrite on
${If} ${RunningX64}
  File "..\x64\Release\autonice.exe"
  File "..\x64\Release\autongui.exe"
${Else}
  File "..\Release\autonice.exe"
  File "..\Release\autongui.exe"
${EndIf}
  CreateDirectory "$SMPROGRAMS\autonice"
  CreateShortCut "$SMPROGRAMS\autonice\autonice.lnk" "$INSTDIR\autongui.exe"
SectionEnd

Section -AdditionalIcons
  CreateShortCut "$SMPROGRAMS\autonice\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\autonice.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\autongui.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
  ExecWait '"$SYSDIR\sc.exe" create autonice type= own start= auto binPath= "$INSTDIR\autonice.exe" obj= LocalSystem DisplayName= "AutoNice"' $0
  IntCmp $0 0 +3
    MessageBox MB_ICONINFORMATION|MB_OK "Cannot register the program as a service."
	Abort
  ExecWait '"$SYSDIR\sc.exe" start autonice' $0
  IntCmp $0 0 +3
    MessageBox MB_ICONINFORMATION|MB_OK "Cannot start the program as a service."
	Abort
SectionEnd

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name)?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  ExecWait '"$SYSDIR\sc.exe" stop autonice' $0
  ExecWait '"$SYSDIR\sc.exe" delete autonice' $0
  IntCmp $0 1060 +4
    IntCmp $0 0 +3
      MessageBox MB_ICONINFORMATION|MB_OK "Cannot delete the service."
      Abort

  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\autongui.exe"
  Delete "$INSTDIR\autonice.exe"

  Delete "$SMPROGRAMS\autonice\Uninstall.lnk"
  Delete "$SMPROGRAMS\autonice\autonice.lnk"

  RMDir "$SMPROGRAMS\autonice"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd