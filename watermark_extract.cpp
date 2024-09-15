#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>

using namespace cv;
using namespace std;

// Function to extract binary data from the LSB of the blue channel
string extractMetadata(const Mat &frame, int metadataSize) {
    string binaryData;
    int dataIdx = 0;

    // Iterate over each pixel and extract the metadata from the blue channel's LSB
    for (int y = 0; y < frame.rows && dataIdx < metadataSize * 8; y++) {
        for (int x = 0; x < frame.cols && dataIdx < metadataSize * 8; x++) {
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
    for (size_t i = 0; i < binaryData.length(); i += 8) {
        bitset<8> b(binaryData.substr(i, 8));
        char c = static_cast<char>(b.to_ulong());
        metadata += c;
    }
    return metadata;
}

int main() {
    // Load the watermarked video file
    string videoFile = "watermarked_video.mp4";
    VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        cout << "Error: Could not open video file." << endl;
        return -1;
    }

    // Metadata size (in characters)
    int metadataSize = 28; // Length of "Your Hidden Watermark Metadata"

    // Read the first frame from the video
    Mat frame;
    cap.read(frame);

    // Extract metadata from the first frame
    string extractedBinary = extractMetadata(frame, metadataSize);

    // Convert the extracted binary data back to metadata
    string extractedMetadata = binaryToMetadata(extractedBinary);

    cout << "Extracted Metadata: " << extractedMetadata << endl;

    // Release resources
    cap.release();

    return 0;
}
