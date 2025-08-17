#!/bin/bash

# Arg check
if [[ $# -ne 1 ]]; then
  echo "Usage: $0 <program_name>"
  exit 1
fi

# Check if perf is installed
if ! command -v perf &> /dev/null; then
  echo "perf is not installed. Please install it first."
  exit 1
fi

# Check if the program exists
if ! command -v "$1" &> /dev/null; then
  echo "Program '$1' not found. Please provide a valid program name."
  exit 1
fi

# Clean up any previous perf data
if [[ -f perf.data ]]; then
  echo "Removing previous perf data..."
  rm perf.data
fi

if [[ -f out.perf ]]; then
  echo "Removing previous perf report..."
  rm out.perf
fi

# Run perf record
echo "Recording performance data for '$1'..."
perf record -g -F 999 "$1"

# Check if perf record was successful
if [[ $? -ne 0 ]]; then
  echo "perf record failed. Please check the program and try again."
  exit 1
fi

# Generate perf report
echo "Generating perf report..."
perf script -F +pid > out.perf
if [[ $? -ne 0 ]]; then
  echo "perf script failed. Please check the recorded data."
  exit 1
fi

# Delete the perf data file
echo "Cleaning up..."
rm perf.data

# Open the report in Firefox
echo "Opening report in Firefox..."
if command -v firefox &> /dev/null; then
  firefox out.perf
else
  echo "Firefox is not installed. Please open 'out.perf' manually."
fi
