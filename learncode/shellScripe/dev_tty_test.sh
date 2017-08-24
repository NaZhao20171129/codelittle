#! /usr/bin/bash

printf "enter new password"
stty -echo

read password < /dev/tty
printf "\n enter again:"
read password2 < /dev/tty

printf "\n"

stty echo

echo "password1 = "  $password
echo "\npassword2 = " $password2

echo "allDone"
