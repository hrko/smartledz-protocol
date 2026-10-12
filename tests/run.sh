#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."
binary=$(mktemp)
trap 'rm -f "$binary"' EXIT
g++ -std=c++17 -Wall -Wextra -Werror -Iinclude src/state_codec.cpp tests/state_codec_test.cpp -o "$binary"
"$binary"
