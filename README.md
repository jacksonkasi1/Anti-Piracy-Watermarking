# AntiPiracyWatermarking

This repository provides a solution to embed invisible watermarks into video files and extract them. The invisible watermark can be metadata, such as text or other information, used to prevent unauthorized distribution and trace pirated content.

## Features
- **Invisible Watermark Embedding**: Hides metadata as an invisible watermark within the video using the Least Significant Bit (LSB) technique.
- **Watermark Extraction**: Extracts the invisible watermark from the video for forensic analysis or traceability.

## Requirements
- **C++ Compiler**
- **OpenCV Library** (version 4.0 or later)
- **FFmpeg (Optional)** for video conversion if needed.

## Installation
1. Install OpenCV if it's not already installed. You can install it via a package manager or build from source:
   - For Ubuntu:
     ```bash
     sudo apt update
     sudo apt install libopencv-dev
     ```
   - For Mac (using Homebrew):
     ```bash
     brew install opencv
     ```

2. Clone this repository:
   ```bash
   git clone https://github.com/jacksonkasi1/Anti-Piracy-Watermarking.git
   cd Anti-Piracy-Watermarking
   ```

## Build Instructions

1. Compile the watermark embedding code:
   ```bash
   g++ -o watermark_embed watermark_embed.cpp `pkg-config --cflags --libs opencv4`
   ```

2. Compile the watermark extraction code:
   ```bash
   g++ -o watermark_extract watermark_extract.cpp `pkg-config --cflags --libs opencv4`
   ```

## How to Use

### 1. Embed Invisible Watermark

To embed a watermark in a video file, follow these steps:

1. **Prepare Your Video**: Ensure that you have a video file (`.mp4`, `.avi`, or other formats supported by OpenCV) that you want to embed the watermark into.
   
2. **Prepare Metadata**: You can embed any text or string data (e.g., `"Your Hidden Watermark Metadata"`) as a watermark.

3. **Run the Embedding Program**:
   ```bash
   ./watermark_embed
   ```
   - The program will embed the metadata into the video file and save the watermarked video as `watermarked_video.mp4`.

### 2. Extract Watermark from Video

To extract the hidden watermark from a video file:

1. **Ensure you have the watermarked video** that contains the hidden metadata (`watermarked_video.mp4`).

2. **Run the Extraction Program**:
   ```bash
   ./watermark_extract
   ```
   - The program will extract the hidden watermark and print it to the console.

### Example Usage

#### Embedding Example
```bash
./watermark_embed
```
This will take your video (specified in the code) and embed the watermark metadata, saving the result as `watermarked_video.mp4`.

#### Extraction Example
```bash
./watermark_extract
```
This will extract the watermark from the `watermarked_video.mp4` and display the metadata in the terminal.

## Code Overview

### `watermark_embed.cpp`
This file contains the code for embedding the watermark:
- **Functionality**: Embeds metadata into the least significant bits (LSB) of the blue channel in each frame of the video.
- **Key Steps**:
  1. Load the video file.
  2. Convert the metadata to a binary string.
  3. Modify the LSB of the blue channel in each pixel to hide the metadata.
  4. Save the watermarked video.

### `watermark_extract.cpp`
This file contains the code for extracting the watermark:
- **Functionality**: Extracts the embedded metadata from the LSB of the blue channel in each frame of the video.
- **Key Steps**:
  1. Load the watermarked video.
  2. Read the LSB of each pixel's blue channel.
  3. Reconstruct the binary string and convert it back to human-readable metadata.
  4. Print the extracted metadata.

## Customization

- **Metadata Size**: Ensure that the size of the metadata (in characters) does not exceed the number of available pixels in the video. If you plan to embed larger metadata, increase the resolution of the video or embed the watermark across multiple frames.

- **Watermark Embedding Locations**: The current implementation uses the blue channel. You can modify the code to embed the watermark in the red or green channel, or use multiple channels for added complexity.

## License
This project is licensed under the MIT License.

---

### Potential Enhancements:
- You can add more advanced watermarking techniques, such as embedding in the frequency domain (DCT, DWT) for greater robustness.
- Implement error-checking and validation for larger metadata sizes.

Feel free to improve the solution and make contributions!