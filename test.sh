#!/bin/sh

export BUILD_TARGET=$(dirname "$0")

function main()
{
	# Build command.

    cmake -B build -G Ninja
    (cd build && ninja)
}

main $@
