#pragma once

using SCOPED_SC_HANDLE = std::unique_ptr<std::remove_pointer<SC_HANDLE>::type, decltype(&::CloseServiceHandle)>;
using SCOPED_HKEY = std::unique_ptr<std::remove_pointer<HKEY>::type, decltype(&::RegCloseKey)>;
using SCOPED_HANDLE = std::unique_ptr<std::remove_pointer<HANDLE>::type, decltype(&::CloseHandle)>;
