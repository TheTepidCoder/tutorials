#Created on 2024-08-24 by TheTepidCoder
#Purpose: To generate a self-signing certificate
#which works with the Microsoft's signtool.exe.
#Usage: ./genkey.sh [certificate_name] [days_until_expiry]
if [ $# -lt 2 ]
then
  echo "Error: A key name and expiry day count must be specified."
  exit;
fi
openssl genrsa 2048 > "${1}_certificate_key.key"
openssl req -new -x509 -nodes -sha256 -days $2 \
-key "${1}_certificate_key.key" -out "${1}_certificate.crt"
winpty openssl pkcs12 -export -out "${1}_certificate.p12" \
-inkey "${1}_certificate_key.key" \
-in "${1}_certificate.crt"
