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
cd build && cmake -DRUN_COVERAGE=1 .. && make


echo ""
echo "  -> Running tests..."
echo ""
ctest --test-dir tests -VV

echo ""
echo "  -> Running coverage..."
echo ""
cd .. && gcovr -r "$PWD"
