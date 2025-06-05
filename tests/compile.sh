#!/bin/sh

if ! command -v avr-gcc >/dev/null 2>&1; then
    echo "avr-gcc not found. Skipping compilation test." >&2
    exit 0
fi

set -e

avr-gcc -mmcu=atmega16 -c 'SmartTap - Phase 2/project.c' -o /dev/null
avr-gcc -mmcu=atmega16 -c 'SmartTap - Phase 3/test.c' -o /dev/null

echo "Compilation succeeded."
