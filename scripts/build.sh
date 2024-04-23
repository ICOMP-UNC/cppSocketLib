# !bin/bash -e

echo ""

if [ -d "./build" ]; then
    echo "  -> Removing old build directory..."
    rm -rf ./build/*
else
    echo "  -> Creating build directory..."
    mkdir build
fi

echo "  -> Building project"
echo ""


cd build && cmake -GNinja -DRUN_TESTS=0 -DRUN_COVERAGE=0 .. && ninja

echo " -> Running project"
./FirstProjectInCMake
