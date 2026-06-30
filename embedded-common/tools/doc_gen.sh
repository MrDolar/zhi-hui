#!/bin/bash
# doc_gen.sh - 文档生成脚本

set -e

echo "Generating documentation..."

# 检查 Doxygen
if ! command -v doxygen &> /dev/null; then
    echo "Doxygen not found. Installing..."
    sudo apt-get install -y doxygen graphviz
fi

# 生成文档
doxygen Doxyfile

echo "Documentation generated in docs/html/"
