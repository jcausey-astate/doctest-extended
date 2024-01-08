#!/usr/bin/env bash

# Get the most recent version of doctest
doctestURL=$(curl -s https://api.github.com/repos/doctest/doctest/releases/latest \
| grep "browser_download_url.*\.h" \
| cut -d : -f 2,3 \
| tr -d \" )

version=$(echo $doctestURL | xargs dirname | xargs basename)
old_version=$(cat upstream/.doctest-version 2>/dev/null)
echo "Current upstream Doctest version is \"${version}\""
echo "Our version of Doctest is is \"${old_version}\""

if [[ $old_version != $version ]] ; then
    echo "New version found... Downloading version ${version}."
    mkdir -p upstream
    wget -qO upstream/"doctest.h" $doctestURL && echo $version > upstream/.doctest-version
else
    echo "Our version is current.  No changes made."
fi
