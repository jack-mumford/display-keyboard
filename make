#!/usr/bin/env bash

base_dir=$(dirname "$0")

PYTHONDONTWRITEBYTECODE=1 exec python3 "$base_dir/make.py" "$@"
