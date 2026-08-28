#!/usr/bin/env bash
# Build every .cpp module (main + CustomerInfo + StaffInfo + UI_Format).
# Usage: ./build-mac.sh
set -euo pipefail
cd "$(dirname "$0")"
c++ -std=c++14 main.cpp CustomerInfo/*.cpp StaffInfo/*.cpp UI_Format/*.cpp -o main
echo "Built ./main — run with: ./main"
