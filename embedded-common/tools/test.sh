#!/bin/bash
# test.sh - 测试脚本

set -e

echo "Running tests..."

# 编译测试
cd build
cmake .. -DBUILD_TESTS=ON
make -j$(nproc)

# 运行测试
./test/test.elf

echo "Tests complete!"
