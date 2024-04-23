<<<<<<< HEAD

# Volver al directorio original# !bin/bash -e
=======
# !bin/bash -e
>>>>>>> 88b5262969388c598d04abefea7296c027e8c15a

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

