#pragma once
#include <vector>

#include "utils/Image.h"

class Solution
{
	using Image = PackedImage;

	int frameTemplate[25] = {
			1, 1, 1, 1, 1,
			1, 0, 0, 0, 1,
			1, 0, 1, 0, 1,
			1, 0, 0, 0, 1,
			1, 1, 1, 1, 1 };

	int internalTemplates[4][9]{
			{0,1,0, 1,0,1, 0,1,0},
			{0,1,0, 0,0,0, 0,1,0},
			{0,0,0, 1,0,1, 0,0,0},
			{1,0,1, 0,0,0, 1,0,1}
	};



public:
	void compute(std::vector<Image> inputImages);

	void markRegionAsProcessed(Image* target, Point regionStart);

	bool pixelIsOfInterest(Pixel pixel) const;

	void applyFiltering(Image* target, Point regionStart, int matchedTemplateNumber);

	void scanRegionOfInterest(Image* target, Point regionStart);

	Point calculateRoiStart(int32_t pixelVectorIndex, int32_t imageWidth);

	bool frameTemplateFound(Image* target, Point regionStart);

	int internalTemplateFound(Image* target, Point regionStart);

	int pointToVectorIndex(Point point, int32_t imageWidth);

	Point vectroIndexToPoint(int32_t vectorIndex, int32_t imageWidth);

	int* combineTemplates(int matchedInnerTemplateNum);
	void scanImagePixels(Image* target);
};
