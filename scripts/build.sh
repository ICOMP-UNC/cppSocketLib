
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

cd build && cmake -GNinja .. && ninja

# echo "  -> Running project"
# echo ""
# ./FirstProjectInCMake

