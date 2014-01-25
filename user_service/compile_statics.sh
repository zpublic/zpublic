#!/bin/sh

find Public/js -type f -print0 | xargs -0 -I FILE bash -c "MIN=Public/scripts/\`basename FILE .js\`.min.js
uglifyjs2 \"FILE\" -o \"\$MIN\"
echo -e \"\tcompileJS FILE\""

find Public/less -type f -print0 | xargs -0 -I FILE bash -c "MIN=Public/styles/\`basename FILE .less\`.min.css
lessc --no-color -x \"FILE\" \"\$MIN\"
echo -e \"\tcompileLESS FILE\""

git add -A
