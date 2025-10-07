#!/bin/bash

echo "=========================================="
echo "Game of Life - Correctness Validation"
echo "=========================================="
echo ""

# Compile
echo "Compiling..."
make clean
make

# Create validation images
echo ""
echo "Creating validation images (8x8 grid, 5 steps)..."
make validate_images

echo ""
echo "=========================================="
echo "Validation Images Created:"
echo "=========================================="
ls -lh *.pgm

echo ""
echo "=========================================="
echo "How to Validate:"
echo "=========================================="
echo ""
echo "1. VIEW IMAGES (requires ImageMagick or display tool):"
echo "   display test_static_init.pgm"
echo "   display test_static_step001.pgm"
echo ""
echo "2. COMPARE IMAGES (check for differences):"
echo "   compare -metric AE test_static_init.pgm test_static_step001.pgm diff.pgm"
echo "   Output: Number of differing pixels (0 = identical)"
echo ""
echo "3. MANUAL VERIFICATION:"
echo "   Open images in any image viewer and verify evolution follows"
echo "   Conway's rules (birth on 3, survival on 2-3 neighbors)"
echo ""
echo "4. COMPARE ORDERED vs STATIC after same steps:"
echo "   compare -metric AE test_ordered_step005.pgm test_static_step005.pgm diff_modes.pgm"
echo "   They SHOULD differ (different algorithms)"
echo ""
echo "5. CONVERT TO PNG for report (optional):"
echo "   convert test_static_init.pgm test_static_init.png"
echo ""
echo "=========================================="
echo
