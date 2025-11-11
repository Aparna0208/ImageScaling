// provided.  do not edit. do not submit.
#ifndef PROVIDED_FUNCTIONS_H_
#define PROVIDED_FUNCTIONS_H_

#include <vector>
#include <string>
#include <cstdint>

// Pixel is a 3-tuple: {red, green, blue}
struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

// Image is a 2D vector of Pixels
using Image = std::vector<std::vector<Pixel>>;

// maximum image width
const size_t MAX_WIDTH = 1920;
// maximum image height
const size_t MAX_HEIGHT = 1080;

/**
 * Opens and validates a PPM file while populating an `Image`.
 * @param filename name of the PPM file to read from
 * @returns a non-empty `Image` in column-major order
 * @throws `std::runtime_error` if file cannot be opened or is invalid
*/
Image load_image(const std::string& filename);

/**
 * Outputs an `Image` to a PPM file
 * @param filename name of the PPM file to write to
 * @param image a non-empty `Image` in column-major order
 * @throws std::invalid_argument if image is empty or file cannot be opened
 */
void output_image(const std::string& filename, const Image& image);

/**
 * Maps given coordinate to source image for interpolation.
 * @param source_dimension width or height of source image
 * @param target_dimension width or height of target image
 * @param pixel_coordinate X- or Y-coordinate in target image
 * @returns X- or Y-coordinate of the location where the pixel maps in the source image
 * @throws std::invalid_argument if dimensions or coordinate are invalid
 */
double map_coordinates(size_t source_dimension,
                       size_t target_dimension,
                       size_t pixel_coordinate);

/**
 * Calculates the color values of a specific pixel.
 * @param source_image A non-empty `Image` in column major order
 * @param x X-coordinate (column) of where a pixel maps in the source image
 * @param y Y-coordinate (row) of where a pixel maps in the source image
 * @returns a `Pixel`
 * @throws std::invalid_argument if coordinates are invalid
 */
Pixel bicubic_interpolation(const Image& source_image, double x, double y);

/**
 * Scales the whole image to new width and height
 * @param source_image A non-empty `Image` in column major order
 * @param target_width width of target image
 * @param target_height height of target image
 * @returns a scaled `Image`
 */
Image scale_image(const Image& source_image,
                  size_t target_width,
                  size_t target_height);

/**
 * performs bicubic interpolation on four pixels
 * @param t the decimal part of the coordinate
 * @param p1 first `Pixel` (left-to-right or top-to-bottom order)
 * @param p2 second `Pixel`
 * @param p3 third `Pixel`
 * @param p4 fourth `Pixel`
 * @returns the interpolated `Pixel`
 */
Pixel bicubic_pixel(double t, const Pixel& p1, const Pixel& p2, const Pixel& p3, const Pixel& p4);

#endif  // PROVIDED_FUNCTIONS_H_
