#include "rtw_stb_image.h"
#include "external\stb_image.h"

rtw_image::rtw_image(const char* image_filename)
{
	auto filename = std::string(image_filename);
	//auto imagedir = getenv("RTW_IMAGES");

	//if (imagedir && load(std::string(imagedir) + "/" + image_filename)) return;
	if (load(filename)) return;
	if (load("images/" + filename)) return;
	if (load("../images/" + filename)) return;
	if (load("../../images/" + filename)) return;
	if (load("../../../images/" + filename)) return;
	if (load("../../../../images/" + filename)) return;
	if (load("../../../../../images/" + filename)) return;
	if (load("../../../../../../images/" + filename)) return;

	std::cerr << "ERROR: could not load image file " << image_filename << ".\n";
}

rtw_image::~rtw_image()
{
	delete[] bdata;
	STBI_FREE(fdata);
}

bool rtw_image::load(const std::string& filename)
{
	auto n = bytes_per_pixel;
	fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);
	if (fdata == nullptr) return false;

	bytes_per_scanline = image_width * bytes_per_pixel;
	convert_to_bytes();
	return true;
}

const unsigned char* rtw_image::pixel_data(int x, int y) const
{
	static unsigned char magenta[] = { 255, 0 , 255 };
	if (bdata == nullptr) return magenta;

	x = clamp(x, 0, image_width);
	y = clamp(y, 0, image_height);

	return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
}

int rtw_image::clamp(int _x, int _low, int _high)
{
	if (_x < _low) return _low;
	if (_x < _high) return _x;
	return _high - 1;

}

unsigned char rtw_image::float_to_byte(float _value)
{
	if (_value <= 0.0)
		return 0;
	if (_value >= 1.0)
		return 255;
	return static_cast<unsigned char>(256.0 * _value);
}

void rtw_image::convert_to_bytes()
{
	int total_bytes = image_width * image_height * bytes_per_pixel;
	bdata = new unsigned char[total_bytes];

	auto* bptr = bdata;
	auto* fptr = fdata;
	for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
		*bptr = float_to_byte(*fptr);
}
