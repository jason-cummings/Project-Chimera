#!/bin/bash

function print_usage {
    echo "Usage: $0 <main_fbx_filename> <hitbox_fbx_filename> [-d]"
    echo "Flags:"
    echo "  -d  run with debug print on"
}

if !([[ $# -ge 2 ]] && echo $1 | grep ".fbx$" -q && echo $2 | grep ".fbx$" -q); then
    print_usage
    exit 1
fi;

DEBUG=""
if echo $* | grep -e "-d" -q; then
    DEBUG="-d"
fi

# Replace the .fbx in the filename
TEXDIR="./$(echo $1 | sed 's/\.fbx$/\.fbm/g')/"
HITTEXDIR="./$(echo $2 | sed 's/\.fbx$/\.fbm/g')/"

# Remove any possibly leftover directories
rm -r ./output/ 2>/dev/null
rm -r $TEXDIR 2>/dev/null
rm -r $HITTEXDIR 2>/dev/null

# ./FbxParser FbxTest2.fbx -d
./bin/bin/FbxParser "$1" "$DEBUG"
./bin/bin/FbxParser "$2" -h "$DEBUG"

mv $TEXDIR ./output/Textures/
rm -r $HITTEXDIR 2>/dev/null
