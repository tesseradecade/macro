#!/bin/bash

# Check library needed for macro testing
# https://libcheck.github.io/check/web/install.html

if [[ "$OS" == *"ubuntu"* ]]; then
  echo "Installing check with Aptitude..."
  sudo apt-get install check
elif [[ "$OS" == *"macos"* ]]; then
  if [[ $(command -v brew) != "" ]]; then
    brew install check
  else
    echo "Brew needs to be installed to build check on OSX"
  fi
else
  echo "Instruction for $OS not found"
  exit 1
fi
