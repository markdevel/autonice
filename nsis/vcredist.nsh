!define MSVCRT2013_X86_NAME "Visual C++ Redistributable Packages for Visual Studio 2013 (x86)"
!define MSVCRT2013_X86_VERSION "12.0.30501.0"
!define MSVCRT2013_X86_DOWNLOAD_URL "http://download.microsoft.com/download/C/6/D/C6DF4616-2D85-43AD-A4FF-C9DC1F94D026/vcredist_x86.exe"
!define MSVCRT2013_X86_FILE_NAME "vcredist_x86.exe"
!define MSVCRT2013_X86_REG_KEY "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{17b12e02-9e0f-435b-a641-6fa68bb60b6d}"
!define MSVCRT2013_X64_NAME "Visual C++ Redistributable Packages for Visual Studio 2013 (x64)"
!define MSVCRT2013_X64_VERSION "12.0.30501.0"
!define MSVCRT2013_X64_DOWNLOAD_URL "http://download.microsoft.com/download/C/6/D/C6DF4616-2D85-43AD-A4FF-C9DC1F94D026/vcredist_x64.exe"
!define MSVCRT2013_X64_FILE_NAME "vcredist_x64.exe"
!define MSVCRT2013_X64_REG_KEY "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{730ca3c6-815d-4b47-abc9-5082acd0267f}"

!macro INSTALL_MSVCRT2013 _n _v _d _f _r
  Push $R0
  ClearErrors
  SetRegView 32
  ReadRegDword $R0 HKLM "${_r}" "BundleVersion"
  SetRegView lastused
  IfErrors 0 +2
    DetailPrint "${_n} was not found."
  StrCmp $R0 "12.0.30501.0" 0 +3
    DetailPrint "${_n} had already installed. skippng."
    Goto done
  DetailPrint "Downloading ${_n} ..."
  NSISdl::download /TIMEOUT=30000 "${_d}" "$TEMP\${_f}"
  Pop $R0
  StrCmp $R0 "success" +3
    MessageBox MB_OK "${_n} could not download."
	Quit
  DetailPrint "Running ${_n} ..."
  ExecWait '"$TEMP\${_f}" /q'
  DetailPrint "Finished ${_n} setup."
  Delete "$TEMP\${_f}"
done:
  Pop $R0
!macroend

!macro INSTALL_MSVCRT2013_X86
  !insertmacro INSTALL_MSVCRT2013 `${MSVCRT2013_X86_NAME}` `${MSVCRT2013_X86_VERSION}` `${MSVCRT2013_X86_DOWNLOAD_URL}` `${MSVCRT2013_X86_FILE_NAME}` `${MSVCRT2013_X86_REG_KEY}`
!macroend

!macro INSTALL_MSVCRT2013_X64
  !insertmacro INSTALL_MSVCRT2013 `${MSVCRT2013_X64_NAME}` `${MSVCRT2013_X64_VERSION}` `${MSVCRT2013_X64_DOWNLOAD_URL}` `${MSVCRT2013_X64_FILE_NAME}` `${MSVCRT2013_X64_REG_KEY}`
!macroend
