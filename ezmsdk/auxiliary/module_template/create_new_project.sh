#!/bin/bash

#author: Quang Hai Nguyen
#version: 1.0.0

echo
echo
echo "#################################"
echo "# module template generation"
echo "# author: Quang Hai Nguyen"
echo "# version: 1.0.0"
echo "#################################"
echo

if [ -z $1 ] || [ -z $2 ]
then
    echo "Please type ./new_project.sh <project_name> <file_path> to create a new template"
else
    echo create module $1 at $2
    
    #Copy and rename the files and folders
    cp -r template $2
    mv $2/template $2/$1
    mv $2/$1/module.c $2/$1/$1.c
    mv $2/$1/module.h $2/$1/$1.h
    
    #Modify the contents of the files to the new module name
    sed -i -e "s/module/$1/g" $2/$1/CMakeLists.txt
    sed -i -e "s/module/$1/g" $2/$1/unit_test/CMakeLists.txt
    sed -i -e "s/module/$1/g" $2/$1/unit_test/unit_test.cpp
    sed -i -e "s/module/$1/g" $2/$1/$1.c
fi
echo
echo
