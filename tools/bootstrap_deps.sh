#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
ROOT_DIR=$(cd -- "$SCRIPT_DIR/.." && pwd)
SDL_DIR="$ROOT_DIR/vendors/SDL"
SDL_COMMIT="8e37db5e797b6167f3a00d697d816a684bd259c7"

fail() {
    echo "ERROR: $*" >&2
    exit 1
}

require_command() {
    local command_name="$1"
    command -v "$command_name" >/dev/null 2>&1 || fail "Required command not found: $command_name"
}

ensure_git_repo() {
    git -C "$ROOT_DIR" rev-parse --show-toplevel >/dev/null 2>&1 || fail "This script must be run from within the repository checkout"
}

bootstrap_submodule() {
    echo "Initializing SDL3 submodule..."
    git -C "$ROOT_DIR" submodule sync -- vendors/SDL
    git -C "$ROOT_DIR" submodule update --init --recursive --checkout vendors/SDL
}

verify_checkout() {
    local head_commit
    head_commit=$(git -C "$SDL_DIR" rev-parse HEAD)
    [[ "$head_commit" == "$SDL_COMMIT" ]] || fail "vendors/SDL is at $head_commit, expected $SDL_COMMIT"
}

echo "Checking required tools..."
require_command git
require_command cmake
require_command ninja

ensure_git_repo
bootstrap_submodule
verify_checkout

echo "Dependency bootstrap complete."
echo "SDL3 pinned at $(git -C "$SDL_DIR" describe --tags --always)"