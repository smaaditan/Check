#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
TP_DIR="$PROJECT_DIR/third_party"
XCPL_DIR="$TP_DIR/XCPlite"

echo "==> Preparing third_party..."
mkdir -p "$TP_DIR"

if [ ! -d "$XCPL_DIR/.git" ]; then
  echo "==> Fetching XCPlite (MIT) ..."
  git clone --depth=1 https://github.com/vectorgrp/XCPlite.git "$XCPL_DIR"
else
  echo "==> Updating XCPlite ..."
  (cd "$XCPL_DIR" && git pull --ff-only)
fi

echo "==> Configuring & building..."
rm -rf "$PROJECT_DIR/build"
cmake -S "$PROJECT_DIR" -B "$PROJECT_DIR/build"
cmake --build "$PROJECT_DIR/build" -j"$(nproc)"

# convenience: copy artifacts to root
cp "$PROJECT_DIR/build/sample_ecu_demo" "$PROJECT_DIR/sample_ecu_demo.out" || true
cp "$PROJECT_DIR/build/sample_ecu_demo.a2l" "$PROJECT_DIR/sample_ecu_demo.a2l" || true

echo
echo "✅ Build done."
echo "Run:  ./sample_ecu_demo.out"
