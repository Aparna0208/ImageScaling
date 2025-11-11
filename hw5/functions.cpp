#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <string>
#include "functions.h"

Image load_image(const std::string& filename) {
    
    if (filename.empty()) {
        throw std::runtime_error("Invalid filename");
    }

    std::ifstream ifs(filename);
    if (!ifs.is_open()) {
        throw std::runtime_error("Failed to open " + filename); 
    }

    std::string ppm_type;
    size_t width = 0;
    size_t height = 0;
    int max_color = 0;

    if (!(ifs >> ppm_type)) {
        throw std::runtime_error("Failed to read type");
    }
    

    if (ppm_type != "P3" && ppm_type != "p3") {
        std::stringstream ss;
        ss << "Invalid type " << ppm_type;
        throw std::runtime_error(ss.str());
    }
    if (!(ifs >> width) || !(ifs >> height)) {
        throw std::runtime_error("Invalid dimensions"); 
    }
    
    if (width == 0 || height == 0 || width > MAX_WIDTH || height > MAX_HEIGHT) {
        throw std::runtime_error("Invalid dimensions");
    }

    if (!(ifs >> max_color)) {
        throw std::runtime_error("Invalid max color");
    }

    if (max_color < 1 || max_color > 255) {
        throw std::runtime_error("Invalid max color");
    }

    Image image(width, std::vector<Pixel>(height));
    int color_val = 0;
    
for (size_t j = 0; j < height; ++j) {    
    for (size_t i = 0; i < width; ++i) {
            
            Pixel p;
      
            for (int color_idx = 0; color_idx < 3; ++color_idx) {
                if (!(ifs >> color_val)) {
                    throw std::runtime_error("Not enough values");
                }
                
                if (color_val < 0 || color_val > max_color) {
                    throw std::runtime_error("Invalid color value");
                }
                
                if (color_idx == 0) p.red = static_cast<uint8_t>(color_val);
                else if (color_idx == 1) p.green = static_cast<uint8_t>(color_val);
                else p.blue = static_cast<uint8_t>(color_val);
            }

            image.at(i).at(j) = p; 
        }
    }
    
    if (ifs >> color_val) {
        throw std::runtime_error("Too many values");
    }
    
    return image;
}

void output_image(const std::string& filename,
                  const Image& image) {
    // TODO(student): implement writing image to file
    if (image.empty() || image.at(0).empty()) {
        throw std::invalid_argument("Invalid image");
    }
    if (filename.empty()) {
        throw std::invalid_argument("Invalid filename");
    }
    size_t width = image.size();
    size_t height = image.at(0).size();

    std::ofstream ofs(filename);
    
    if (!ofs.is_open()) {
        throw std::invalid_argument("Failed to open " + filename);
    }
    ofs << "P3\n";
    ofs << width << " " << height << "\n";
    ofs << "255\n";

for (size_t j = 0; j < height; ++j) {
        for (size_t i = 0; i < width; ++i) {
            const Pixel& p = image.at(i).at(j); 
            
            ofs << static_cast<int>(p.red) << " "
                << static_cast<int>(p.green) << " "
                << static_cast<int>(p.blue);

            if (i == width - 1) {
                ofs << "\n";
            } else {
                ofs << " ";  
            }
        }
    }
}

double map_coordinates(size_t source_dimension,
                       size_t target_dimension,
                       size_t pixel_coordinate) {
    // TODO(student): implement mapping function.
    if (source_dimension == 0 || target_dimension <= 1) {
        throw std::invalid_argument("Invalid dimension");
    }

    if (pixel_coordinate >= target_dimension) {
        throw std::invalid_argument("Invalid coordinate");
    }
    double scale_factor = (static_cast<double>(source_dimension) - 1.0) /
                          (static_cast<double>(target_dimension) - 1.0);
    
    return scale_factor * static_cast<double>(pixel_coordinate);
   // return {};
}

Pixel bicubic_interpolation(const Image& source_image,
                            double x,
                            double y) {
    // TODO(student): Implement bicubic interpolation
    if (source_image.empty() || source_image.at(0).empty()) {
        throw std::invalid_argument("Invalid image");
    }

    size_t source_width = source_image.size();
    size_t source_height = source_image.at(0).size();

    if (x < 0.0 || y < 0.0 || 
        x >= static_cast<double>(source_width) || 
        y >= static_cast<double>(source_height)) {
        
        throw std::invalid_argument("Invalid coordinate");
    }

    int i2 = static_cast<int>(std::floor(x));
    int j2 = static_cast<int>(std::floor(y));

    int i1 = i2 - 1;
    int i3 = i2 + 1;
    int i4 = i2 + 2;
    
    int j1 = j2 - 1;
    int j3 = j2 + 1;
    int j4 = j2 + 2;

    double t_x = x - static_cast<double>(i2);
    double t_y = y - static_cast<double>(j2);

        auto get_pixel_clamped = [&](int i, int j) -> const Pixel& {
        int clamped_i = std::max(0, std::min(i, static_cast<int>(source_width) - 1));
        int clamped_j = std::max(0, std::min(j, static_cast<int>(source_height) - 1));
        
        return source_image.at(clamped_i).at(clamped_j);
    };

   Pixel r1, r2, r3, r4;


    r1 = bicubic_pixel(t_x, get_pixel_clamped(i1, j1), get_pixel_clamped(i2, j1), get_pixel_clamped(i3, j1), get_pixel_clamped(i4, j1));
    r2 = bicubic_pixel(t_x, get_pixel_clamped(i1, j2), get_pixel_clamped(i2, j2), get_pixel_clamped(i3, j2), get_pixel_clamped(i4, j2));
    r3 = bicubic_pixel(t_x, get_pixel_clamped(i1, j3), get_pixel_clamped(i2, j3), get_pixel_clamped(i3, j3), get_pixel_clamped(i4, j3));
    r4 = bicubic_pixel(t_x, get_pixel_clamped(i1, j4), get_pixel_clamped(i2, j4), get_pixel_clamped(i3, j4), get_pixel_clamped(i4, j4));

    return bicubic_pixel(t_y, r1, r2, r3, r4);
}

Image scale_image(const Image& source_image,
                  size_t target_width,
                  size_t target_height) {
    if (source_image.empty() || source_image.at(0).empty()) {
        throw std::invalid_argument("Invalid image");
    }

    if (target_width == 0 || target_height == 0 ||
        target_width > MAX_WIDTH || target_height > MAX_HEIGHT) {
        
        throw std::invalid_argument("Invalid dimension");
    }
    Image target_image(target_width, std::vector<Pixel>(target_height));

    for (size_t i = 0; i < target_width; ++i) { 
        for (size_t j = 0; j < target_height; ++j) { 
            
            double source_x = map_coordinates(source_image.size(), 
                                              target_width, 
                                              i);

            double source_y = map_coordinates(source_image.at(0).size(), 
                                              target_height, 
                                              j);
            
            Pixel new_pixel = bicubic_interpolation(source_image, source_x, source_y);

            target_image.at(i).at(j) = new_pixel;
        }
    }

    return target_image;
}

