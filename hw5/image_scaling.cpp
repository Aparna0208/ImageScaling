#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include "functions.h"

using std::cout, std::endl, std::cin, std::string, std::stringstream;

int main() {
    // 2D vectors of pixels
    Image source_image;
    Image target_image;

    // declare variables
    string filename;
    unsigned int target_width = 0;
    unsigned int target_height = 0;

    // accept filename
    cout << "Input filename: ";
    cin >> filename;

    try {
        // attempt to load image
        source_image = load_image(filename);
        cout << "Detected image width: " << source_image.size() << endl;
        cout << "Detected image height: " << source_image.at(0).size() << endl;
    } catch (std::exception &ex) {
        cout << "Exception thrown: " << ex.what() << endl;
        return 1;  // exit with error
    }

    // accept target dimensions
    cout << "Input target width: ";
    cin >> target_width;
    cout << "Input target height: ";
    cin >> target_height;

    // TODO(student): Add code to validate input.
    //                Exit program with error if there are problems with
    //                target_width or target_height


if (target_width <= 0 || target_height <= 0 || 
    target_width >= MAX_WIDTH || target_height >= MAX_HEIGHT) {
    
    std::cout << "Invalid target dimensions" << std::endl;
    return 1; 
}
    // END input validation. Do not modify code beyond this point.
    cout << "Start image scaling...\n";
    target_image = scale_image(source_image, target_width, target_height);

    // Final file output.
    try {
        // attempt to output image
        stringstream ss;
        ss << "Scaled" << target_width << "X" << target_height << ".ppm";
        output_image(ss.str().c_str(), target_image);
    } catch (std::exception &ex) {
        cout << "Exception thrown: " << ex.what() << endl;
        // exit with an error
        return 1;
    }

    cout << "Image output\n";

    // exit without errors
    return 0;
}
