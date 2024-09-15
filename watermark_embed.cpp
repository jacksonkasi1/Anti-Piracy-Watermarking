// watermark_embed.cpp

#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>
#include <string>

using namespace cv;
using namespace std;

// Function to convert string metadata to binary (bitset)
string metadataToBinary(const string &metadata) {
    string binaryData;
    for (char c : metadata) {
        bitset<8> b(c);  // Convert each char to a binary representation
        binaryData += b.to_string();
    }
    return binaryData;
}

// Function to convert integer to binary string (32 bits)
string intToBinary(int num) {
    bitset<32> b(num);
    return b.to_string();
}

// Function to embed binary data into the least significant bit (LSB) of the blue channel
Mat embedMetadata(const Mat &frame, const string &dataBinary, int repetitions = 5) {
    Mat watermarkedFrame = frame.clone();
    int dataSize = dataBinary.length();

    // Calculate total bits to embed considering repetitions
    int totalBits = dataSize * repetitions;
    int dataIdx = 0;

    // Iterate over each pixel and embed the metadata into the blue channel's LSB
    for (int y = 0; y < frame.rows && dataIdx < totalBits; y++) {
        for (int x = 0; x < frame.cols && dataIdx < totalBits; x++) {
            Vec3b pixel = watermarkedFrame.at<Vec3b>(y, x);
            // Determine which bit to embed
            int bitPosition = dataIdx % dataSize;
            char bit = dataBinary[bitPosition];
            // Embed the current bit of metadata into the LSB of the blue channel
            uchar blueChannel = pixel[0];
            blueChannel = (blueChannel & ~1) | (bit - '0'); // Set LSB
            pixel[0] = blueChannel; // Update blue channel
            watermarkedFrame.at<Vec3b>(y, x) = pixel;
            dataIdx++;
        }
    }

    return watermarkedFrame;
}

int main() {
    // Load the video file
    string videoFile = "normal_video.mp4";
    VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        cout << "Error: Could not open video file: " << videoFile << endl;
        return -1;
    }

    // Get video properties
    int frameWidth = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(CAP_PROP_FPS);
    int totalFrames = static_cast<int>(cap.get(CAP_PROP_FRAME_COUNT));

    // Output video writer
    // Using 'MJPG' codec for better compatibility
    int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');
    string outputFile = "watermarked_video.avi";
    VideoWriter writer(outputFile, fourcc, fps, Size(frameWidth, frameHeight));

    if (!writer.isOpened()) {
        cout << "Error: Could not open the output video for write: " << outputFile << endl;
        return -1;
    }

    // Metadata to be embedded
    string metadata = "Test"; // You can change this to your desired metadata
    // Convert metadata to binary
    string metadataBinary = metadataToBinary(metadata);
    // Convert metadata length to binary (32 bits)
    int metadataLength = metadata.length();
    string metadataLengthBinary = intToBinary(metadataLength);
    // Concatenate length and metadata binary
    string combinedBinary = metadataLengthBinary + metadataBinary;

    // Repeat the combined binary to increase robustness
    int repetitions = 5;
    string robustBinary = "";
    for(int i = 0; i < repetitions; i++) {
        robustBinary += combinedBinary;
    }

    // Debug: Print the binary data being embedded
    cout << "Metadata Length (binary): " << metadataLengthBinary << endl;
    cout << "Metadata (binary): " << metadataBinary << endl;
    cout << "Embedding metadata with " << repetitions << " repetitions." << endl;

    // Check if the video has enough pixels to embed the combined binary data
    if (robustBinary.length() > frameWidth * frameHeight) {
        cout << "Error: Metadata is too large to embed in a single frame." << endl;
        return -1;
    }

    // Loop through the video frames
    Mat frame;
    int currentFrame = 0;
    while (cap.read(frame)) {
        Mat watermarkedFrame = frame.clone();

        if (currentFrame == 0) {
            // Embed metadata into the first frame
            watermarkedFrame = embedMetadata(frame, robustBinary, repetitions);
            cout << "Embedding metadata into frame " << currentFrame << endl;
        }

        // Write the frame to the output video
        writer.write(watermarkedFrame);
        currentFrame++;
    }

    // Release resources
    cap.release();
    writer.release();

    cout << "Watermarked video has been saved as '" << outputFile << "'." << endl;

    return 0;
}
