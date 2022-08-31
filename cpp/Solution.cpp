#include "Solution.h"

#include "utils/EyePatterns.h"

void Solution::compute(std::vector<Image> inputImages)
{
}

void Solution::markRegionAsProcessed(Image* target, Point regionStart, int32_t imageWidth)
{
	Point currentPixel = Point(regionStart.row, regionStart.column);
	for (int rowOffset = 0; rowOffset < EYE_PATTERN_COL_SIZE; ++rowOffset)
	{
		currentPixel.row += rowOffset;
		for (int colOffset = 0; colOffset < EYE_PATTERN_COL_SIZE; ++colOffset)
		{
			currentPixel.column += colOffset;

			int vectorIndex = pointToVectorIndex(currentPixel, imageWidth);
			target->pixels[vectorIndex].isProcessed = true;
		}
	}
}

bool Solution::pixelIsOfInterest(Pixel pixel) const
{
	return pixel.red >= 200 && !pixel.isProcessed;
}

void Solution::applyFiltering(Image* target, Point regionStart, int32_t imageWidth)
{
	//Note: Copy paste code as "markRegionAsProcessed" function. Could be optimized.
	Point currentPixel = Point(regionStart.row, regionStart.column);
	for (int rowOffset = 0; rowOffset < EYE_PATTERN_COL_SIZE; ++rowOffset)
	{
		currentPixel.row += rowOffset;
		for (int colOffset = 0; colOffset < EYE_PATTERN_COL_SIZE; ++colOffset)
		{
			currentPixel.column += colOffset;

			int vectorIndex = pointToVectorIndex(currentPixel, imageWidth);
			target->pixels[vectorIndex].red -= 150;
		}
	}
}

void Solution::scanRegionOfInterest(Image* target, Point regionStart, int32_t imageHeight, int32_t imageWidth)
{
	int regionDimension = 2 * EYE_PATTERN_COL_SIZE - 1;
	Point regionEnd = Point(regionStart.row + regionDimension, regionStart.column + regionDimension);

	int availableRowShifts = EYE_PATTERN_COL_SIZE;
	int availableColShifts = EYE_PATTERN_COL_SIZE;

	if (regionEnd.row > imageHeight)
	{
		int overflowAmount = imageHeight - regionEnd.row;
		availableRowShifts -= overflowAmount;
	}

	if (regionEnd.column > imageWidth)
	{
		int overflowAmount = imageWidth - regionEnd.column;
		availableColShifts -= overflowAmount;
	}

	Point subregionStart = Point(regionStart.row, regionStart.column);

	bool scanComplete = false;

	for (int rowOffset = 0; rowOffset < availableRowShifts; ++rowOffset)
	{
		subregionStart.row += rowOffset;

		if (scanComplete) { break; }

		for (int colOffset = 0; colOffset < availableColShifts; ++colOffset)
		{
			subregionStart.column += colOffset;

			if (frameTemplateFound(target, subregionStart))
			{
				int potentialInternalTemplate = internalTemplateFound(target, subregionStart);

				if(potentialInternalTemplate != -1)
				{
					applyFiltering(target, subregionStart, imageWidth);
					markRegionAsProcessed(target, regionStart, imageWidth);
					scanComplete = true;
				}
			}
		}
	}





}

Point Solution::calculateRoiStart(int32_t pixelVectorIndex, int32_t imageWidth)
{
	Point pixelPoint = vectroIndexToPoint(pixelVectorIndex, imageWidth);

	int32_t startRow = pixelPoint.row - EYE_PATTERN_COL_SIZE;
	int32_t startCol = pixelPoint.column - EYE_PATTERN_COL_SIZE;

	if (startCol < 0) { startCol = pixelPoint.column; }
	if (startRow < 0) { startRow = pixelPoint.row; }

	return Point(startRow, startCol);
}

bool Solution::frameTemplateFound(Image* target, Point regionStart)
{

}

int Solution::internalTemplateFound(Image* target, Point regionStart)
{
}

int Solution::pointToVectorIndex(Point point, int32_t imageWidth)
{
	return point.row * imageWidth + point.column;
}

Point Solution::vectroIndexToPoint(int32_t vectorIndex, int32_t imageWidth)
{
	int32_t column = vectorIndex % imageWidth;
	int32_t row = vectorIndex - column / imageWidth;
	return Point(row, column);
}
