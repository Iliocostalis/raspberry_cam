#include "imageProcessing.h"
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

namespace ImageProcessing
{
    int val = 0;
    auto last = std::chrono::high_resolution_clock::now();

    std::chrono::microseconds lasts[5];
    int ind = 0;

    std::array<std::vector<char>, IMAGE_BUFFER_COUNT> imageBuffer;
    std::atomic<int> currentImageIndex = 0;
    std::array<XImage*, IMAGE_BUFFER_COUNT> images;

    void init()
    {
#if DEFINED(SHOW_PREVIEW)
        for(auto& image : imageBuffer)
            image.resize(IMAGE_WIDTH * IMAGE_HEIGHT * 4);
#endif
    }

    void process(uint8_t* data, size_t size)
    {
#if DEFINED(SHOW_PREVIEW)
        int nextImageIndex = (currentImageIndex + 1) % imageBuffer.size();
        for(int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i)
        {
            imageBuffer[nextImageIndex][i * 4 + 0] = data[i];
            imageBuffer[nextImageIndex][i * 4 + 1] = data[i];
            imageBuffer[nextImageIndex][i * 4 + 2] = data[i];
            imageBuffer[nextImageIndex][i * 4 + 3] = data[i];
        }
        currentImageIndex = nextImageIndex;
#endif



        auto now = std::chrono::high_resolution_clock::now();
        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(now - last);
        last = now;

        lasts[ind] = microseconds;
        ind = (ind + 1) % 5;

        int64_t averageTime = 0;
        for(int i = 0; i < 5; ++i)
            averageTime += lasts[i].count();

        averageTime /= 5;


        std::cout << "image count: " << val << std::endl;
        std::cout << "fps: " << 1000000.0f / averageTime << std::endl;
        std::cout << "size: " << size << std::endl;

        // Y420
        if(val == 32 && false)
        {
            std::string fileName = std::string("image_") + std::to_string(val);
            
            auto myfile = std::ofstream(fileName, std::ios::out | std::ios::binary);
            //myfile << "P6" << "\n" << 640 << " " << 480 << "\n" << 255 << "\n";
            //size = 640 * 480 / 2;
            //for(int i = 0; i < size; ++i)
            //{
            //    uint8_t valA = (data[i] & 0xf0);
            //    uint8_t valB = (data[i] & 0x0f) << 4;
//
            //    myfile.write((char*)&valA, 1);
            //    myfile.write((char*)&valA, 1);
            //    myfile.write((char*)&valA, 1);
            //    myfile.write((char*)&valB, 1);
            //    myfile.write((char*)&valB, 1);
            //    myfile.write((char*)&valB, 1);
            //}

            myfile << "P6" << "\n" << 1280 << " " << 960 << "\n" << 255 << "\n";
            size = 1280 * 960;
            for(int i = 0; i < size; ++i)
            {
                uint8_t valA = data[i];
                myfile.write((char*)&valA, 1);
                myfile.write((char*)&valA, 1);
                myfile.write((char*)&valA, 1);
            }
            myfile.close();
        }

        ++val;
    }
}