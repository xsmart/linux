#!/bin/bash
#name: create_dir.sh
#author: lei

# check that the directory exists
# @param $1:directory you want to check
function check_path()
{
    dir=$1

    if [ ! -d $dir ]; then
        echo "directory $dir is not existed."
        return 1
    fi
}

# make a directory of a specified size
# @param $1:image file  
#        $2:directory 
#        $3:directory size(M)
function make_dir()
{
    file=$1
    dir=$2
    size=$3
    dd if=/dev/zero of=$file bs=1M count=$size
    mkfs.ext4 $file

    check_path $dir
    if [ $? -eq 1 ]; then
        echo "Now create directory: $dir"
        mkdir -p $dir
    fi
    sudo mount -t ext4 -o loop $file $dir
}


if [ $# -ne 3 ]; then
    echo "parament error,need 3 paraments:"    
    echo "image file, directory, size"
    exit 1
fi
make_dir $1 $2 $3
