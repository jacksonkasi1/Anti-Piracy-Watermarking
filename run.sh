#!/bin/bash

# Enable strict mode
set -euo pipefail

# Function to compile a C++ program
compile_program() {
    local source_file=$1
    local output_file=$2
    echo "Compiling $source_file..."
    g++ "$source_file" -o "$output_file" `pkg-config --cflags --libs opencv4`
    echo "Compiled $source_file to $output_file."
}

# Function to run a program
run_program() {
    local program=$1
    echo "Running $program..."
    ./"$program"
    echo "Finished running $program."
}

# Compile watermark_embed.cpp
compile_program "watermark_embed.cpp" "watermark_embed"

# Run watermark_embed
run_program "watermark_embed"

# Compile watermark_extract.cpp
compile_program "watermark_extract.cpp" "watermark_extract"

# Run watermark_extract
run_program "watermark_extract"

echo "All tasks completed successfully."
