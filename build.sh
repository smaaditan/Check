#!/bin/bash
set -e

# Clean and prepare build directory
rm -rf build
mkdir build

# Run CMake & Build
cd build
cmake ..
make -j$(nproc)

# Copy final binary and A2L to project root
cp sample_ecu_demo ../sample_ecu_demo.out
cd ..

echo "✅ Build complete!"
echo "Run ./sample_ecu_demo.out to start the ECU server."
