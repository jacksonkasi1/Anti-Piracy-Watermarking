// watermark_extract.cpp

#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>
#include <string>
#include <unordered_map>

using namespace cv;
using namespace std;

// Function to extract binary data from the LSB of the blue channel
string extractMetadata(const Mat &frame, int numBits, int startPixel = 0) {
    string binaryData;
    int dataIdx = 0;

    // Iterate over each pixel and extract the metadata from the blue channel's LSB
    for (int y = 0; y < frame.rows && dataIdx < numBits; y++) {
        for (int x = 0; x < frame.cols && dataIdx < numBits; x++) {
            int pixelNumber = y * frame.cols + x;
            if (pixelNumber < startPixel)
                continue; // Skip until startPixel

            Vec3b pixel = frame.at<Vec3b>(y, x);
            uchar blueChannel = pixel[0];
            binaryData += (blueChannel & 1) ? '1' : '0';  // Extract LSB
            dataIdx++;
        }
    }

    return binaryData;
}

// Function to convert binary string to human-readable text
string binaryToMetadata(const string &binaryData) {
    string metadata;
    for (size_t i = 0; i + 8 <= binaryData.length(); i += 8) {
        bitset<8> b(binaryData.substr(i, 8));
        char c = static_cast<char>(b.to_ulong());
        metadata += c;
    }
    return metadata;
}

// Function to convert binary string to integer (32 bits)
int binaryToInt(const string &binaryData) {
    if (binaryData.length() != 32) {
        cout << "Error: Binary data length is not 32 bits." << endl;
        return -1;
    }
    bitset<32> b(binaryData);
    return static_cast<int>(b.to_ulong());
}

int main() {
    // Load the watermarked video file
    string videoFile = "watermarked_video.avi"; // Updated to AVI
    VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        cout << "Error: Could not open video file: " << videoFile << endl;
        return -1;
    }

    // Read the first frame from the video
    Mat frame;
    if (!cap.read(frame)) {
        cout << "Error: Could not read the first frame from the video." << endl;
        return -1;
    }

    // First, extract the metadata length (32 bits) from the first 32 pixels
    string lengthBinary = extractMetadata(frame, 32, 0);
    // Debug: Print the extracted length binary
    cout << "Extracted Metadata Length (binary): " << lengthBinary << endl;
    int metadataLength = binaryToInt(lengthBinary);

    if (metadataLength <= 0 || metadataLength > 1000) { // Arbitrary upper limit
        cout << "Error: Extracted metadata length is invalid: " << metadataLength << endl;
        return -1;
    }

    // Now, extract the metadata bits (metadataLength * 8 bits) starting from pixel 32
    string metadataBinary = extractMetadata(frame, metadataLength * 8, 32);
    // Debug: Print the extracted metadata binary
    cout << "Extracted Metadata (binary): " << metadataBinary << endl;

    // Convert the extracted binary data back to metadata
    string extractedMetadata = binaryToMetadata(metadataBinary);

    cout << "Extracted Metadata: " << extractedMetadata << endl;

    // Release resources
    cap.release();

    return 0;
}
