#!/bin/bash
set -eu

## Remove venv
echo "Removing python venv"
rm -rf venv
echo "Removing build files"
rm -rf build

echo "Removal complete!"