#include <opencv2/opencv.hpp>
#include <iostream>
#include <bitset>

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

// Function to embed metadata in the least significant bit (LSB) of the blue channel
Mat embedMetadata(const Mat &frame, const string &metadataBinary) {
    Mat watermarkedFrame = frame.clone();
    int dataIdx = 0;

    // Iterate over each pixel and embed the metadata into the blue channel's LSB
    for (int y = 0; y < frame.rows && dataIdx < metadataBinary.length(); y++) {
        for (int x = 0; x < frame.cols && dataIdx < metadataBinary.length(); x++) {
            Vec3b pixel = watermarkedFrame.at<Vec3b>(y, x);
            // Embed the current bit of metadata into the LSB of the blue channel
            uchar blueChannel = pixel[0];
            blueChannel = (blueChannel & ~1) | (metadataBinary[dataIdx] - '0'); // Set LSB
            pixel[0] = blueChannel; // Update blue channel
            watermarkedFrame.at<Vec3b>(y, x) = pixel;
            dataIdx++;
        }
    }

    return watermarkedFrame;
}

int main() {
    // Load the video file
    string videoFile = "path_to_your_video.mp4";
    VideoCapture cap(videoFile);

    if (!cap.isOpened()) {
        cout << "Error: Could not open video file." << endl;
        return -1;
    }

    // Get video properties
    int frameWidth = static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH));
    int frameHeight = static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT));
    int fps = static_cast<int>(cap.get(CAP_PROP_FPS));

    // Output video writer
    VideoWriter writer("watermarked_video.mp4", VideoWriter::fourcc('M', 'P', '4', 'V'), fps, Size(frameWidth, frameHeight));

    if (!writer.isOpened()) {
        cout << "Error: Could not open the output video for write." << endl;
        return -1;
    }

    // Metadata to be embedded
    string metadata = "Your Hidden Watermark Metadata";
    string metadataBinary = metadataToBinary(metadata);

    // Loop through the video frames
    Mat frame;
    while (cap.read(frame)) {
        // Embed metadata into the current frame
        Mat watermarkedFrame = embedMetadata(frame, metadataBinary);

        // Write the frame with the embedded metadata to the output video
        writer.write(watermarkedFrame);
    }

    // Release resources
    cap.release();
    writer.release();

    cout << "Watermarked video has been saved as 'watermarked_video.mp4'." << endl;

    return 0;
}
