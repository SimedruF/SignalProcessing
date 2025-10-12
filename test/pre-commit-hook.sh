#!/bin/bash

###############################################################################
# Git Pre-commit Hook
# Runs tests before allowing commit to ensure code quality
#
# Installation:
#   cp pre-commit-hook.sh .git/hooks/pre-commit
#   chmod +x .git/hooks/pre-commit
###############################################################################

echo "Running pre-commit tests..."
echo ""

# Navigate to test directory
cd "$(git rev-parse --show-toplevel)/test" || exit 1

# Run quick test
./quick_test.sh

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ All tests passed. Proceeding with commit..."
    exit 0
else
    echo ""
    echo "✗ Tests failed! Commit aborted."
    echo ""
    echo "Please fix the failing tests before committing."
    echo "You can run './test/run_all_tests.sh' for detailed error information."
    echo ""
    echo "To commit anyway (not recommended), use: git commit --no-verify"
    exit 1
fi
