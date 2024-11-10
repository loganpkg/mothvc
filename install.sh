#! /bin/sh

#
# Copyright (c) 2024 Logan Ryan McLintock
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#

# Build and install script for mothvc

set -e
set -u
set -x

install_dir="$HOME"/bin
flags='-ansi -g -Og -Wall -Wextra -pedantic'


if [ "$(uname)" = Linux ]
then
    indent=indent
else
    indent=gindent
fi

repo_dir=$(pwd)
build_dir=$(mktemp -d)

# Copy files
find . -type f ! -path '*.git*' -exec cp -p '{}' "$build_dir" \;

cd "$build_dir" || exit 1

# Long lines
find . -type f ! -path '*.git*' ! -name '*~' \
    -exec grep -H -n -E '.{80}' '{}' \;

"$indent" -nut -kr tree_to_text.c

cc $flags tree_to_text.c -o tree_to_text

shellcheck -e SC3043 -e SC2034 mothvc

mkdir -p "$install_dir"

mv tree_to_text mothvc "$install_dir"/

# Update files
find . -type f \( -name '*.h' -o -name '*.c' \) -exec cp -p '{}' "$repo_dir" \;
