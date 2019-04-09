#pragma once

#define VERSION_MAJOR ${Major}
#define VERSION_MINOR ${Minor}
#define VERSION_BUILD ${Build}
#define VERSION_REVISION ${Revision}

#define VERSION_PRODUCTNAME_VALUE "${ProductName}"
#define VERSION_COMPANYNAME_VALUE "${CompanyName}"
#define VERSION_LEGALCOPYRIGHT_VALUE "${Copyright}"
#define VERSION_LEGALTRADEMARK_VALUE "${Trademark}"
#define VERSION_VALUE "${Major}.${Minor}.${Build}.${Revision}"

#define BINARY_VERSION ${Major},${Minor},${Build},${Revision}

#define VERSION_FILEVERSION BINARY_VERSION
#define VERSION_PRODUCTVERSION BINARY_VERSION
#define VERSION_FILEVERSION_VALUE VERSION_VALUE
#define VERSION_PRODUCTVERSION_VALUE VERSION_VALUE
