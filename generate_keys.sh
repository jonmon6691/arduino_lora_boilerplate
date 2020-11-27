#!/bin/bash
salt=$(dd if=/dev/urandom bs=8 count=1 2>/dev/null | xxd -ps -u)
secret=$(dd if=/dev/urandom bs=16 count=1 2>/dev/null | xxd -ps -u)

touch secrets.h
chmod 600 secrets.h

echo "#ifndef SECRETS_H" > secrets.h
echo "#define SECRETS_H" >> secrets.h

echo "#define SECRET \"$secret\"" >> secrets.h

echo "#endif" >> secrets.h

echo
echo " [DONE] Secrets generated"
