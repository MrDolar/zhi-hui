#!/bin/bash
# build.sh - 构建脚本

set -e

# 默认平台
PLATFORM=${1:-linux}

echo "Building for platform: $PLATFORM"

# 创建构建目录
mkdir -p build
cd build

# 配置
cmake .. -DPLATFORM=$PLATFORM

# 编译
make -j$(nproc)

echo "Build complete!"
