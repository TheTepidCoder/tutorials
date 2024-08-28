REM Created on 2024-08-24 by TheTepidCoder
REM Purpose: To invoke the Microsoft signtool.exe application
REM to sign a specified executable file with a given certificate
REM file and password.
REM Note: This should be used in the developer command prompt
REM for convenience. Otherwise, it will need to be invoked from
REM the same directory as signtool.exe and provided the full
REM paths to the certificate file and executable file to sign
REM therewith.
REM If less than three arguments were passed, then we can't
REM sign the executable with a certificate and password.
if "%~3" == "" (
  echo "Usage: sign.bat [certificate_name].p12 [password] [executable_to_sign].exe"
  goto :done
)
signtool sign ^
/fd SHA256 ^
/t http://timestamp.comodoca.com ^
/f %1 ^
/p %2 ^
%3
REM Batch file endpoint which will be jumped to on usage error.
:done
