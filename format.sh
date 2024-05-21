# Apply clang-format in all '.c' and '.h' files.
#!/bin/bash

clang-format -i `find . -name '*.c' -o -name '*.h'`

echo "All '.c' and '.h' files formatted."
