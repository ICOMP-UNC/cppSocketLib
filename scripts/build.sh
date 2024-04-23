
# !bin/bash -e


echo ""

if [ -d "./build" ]; then
    echo "  -> Removing old build directory..."
    rm -rf ./build/*
else
    echo "  -> Creating build directory..."
    mkdir build
fi

echo "  -> Compiling project"
echo ""
cd build && cmake -GNinja -DRUN_TESTS=1 -DRUN_COVERAGE=1 .. && ninja
