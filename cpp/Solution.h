#pragma once
#include <vector>

#include "utils/Image.h"

class Solution
{
	using Image = PackedImage;
public:
	void compute(std::vector<Image> inputImages);

	void markRegionAsProcessed(Image* target, Point regionStart, int32_t imageWidth);
	
	bool pixelIsOfInterest(Pixel pixel) const;

	void applyFiltering(Image* target, Point regionStart, int32_t imageWidth);

	void scanRegionOfInterest(Image* target, Point regionStart, int32_t imageHeight, int32_t imageWidth);

	Point calculateRoiStart(int32_t pixelVectorIndex, int32_t imageWidth);

	bool frameTemplateFound(Image* target, Point regionStart);

	int internalTemplateFound(Image* target, Point regionStart);

	int pointToVectorIndex(Point point, int32_t imageWidth);
	Point vectroIndexToPoint(int32_t vectorIndex, int32_t imageWidth);
};
