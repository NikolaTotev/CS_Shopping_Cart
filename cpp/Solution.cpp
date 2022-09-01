#include "Solution.h"

#include "utils/EyePatterns.h"

void Solution::compute(std::vector<Image> inputImages)
{
	for (int imageNumber = 0; imageNumber < inputImages.size(); ++imageNumber)
	{
		scanImagePixels(&inputImages[imageNumber]);
	}
}


void Solution::scanImagePixels(Image* target)
{
	for (int pixelIndex = 0; pixelIndex < target->pixels.size(); ++pixelIndex)
	{
		if(pixelIsOfInterest(target->pixels[pixelIndex]))
		{
			scanRegionOfInterest(target, calculateRoiStart(pixelIndex, target->resolution.width));
		}
	}
}

void Solution::markRegionAsProcessed(Image* target, Point regionStart)
{
	Point currentPixel = Point(regionStart.row, regionStart.column);
	for (int rowOffset = 0; rowOffset < EYE_PATTERN_COL_SIZE; ++rowOffset)
	{
		currentPixel.row += rowOffset;
		for (int colOffset = 0; colOffset < EYE_PATTERN_COL_SIZE; ++colOffset)
		{
			currentPixel.column += colOffset;

			int vectorIndex = pointToVectorIndex(currentPixel, target->resolution.width);
			target->pixels[vectorIndex].isProcessed = true;
		}
	}
}

bool Solution::pixelIsOfInterest(Pixel pixel) const
{
	return pixel.red >= 200 && !pixel.isProcessed;
}

void Solution::applyFiltering(Image* target, Point regionStart, int matchedTemplateNumber)
{
	//Note: Copy paste code as "markRegionAsProcessed" function. Could be optimized.

	int* completeTemplate = combineTemplates(matchedTemplateNumber);

	Point currentPixel = Point(regionStart.row, regionStart.column);
	for (int rowOffset = 0; rowOffset < EYE_PATTERN_COL_SIZE; ++rowOffset)
	{
		currentPixel.row += rowOffset;
		for (int colOffset = 0; colOffset < EYE_PATTERN_COL_SIZE; ++colOffset)
		{
			currentPixel.column += colOffset;

			int templateIndex = pointToVectorIndex(Point(rowOffset, colOffset), 5);
			int pixelIndex = pointToVectorIndex(currentPixel, target->resolution.width);

			if(completeTemplate[templateIndex]==1 && pixelIsOfInterest(target->pixels[pixelIndex]))
			{
				target->pixels[pixelIndex].red -= 150;
			}
		}
	}
}

int* Solution::combineTemplates(int matchedInnerTemplateNum)
{
	int completeTemplate[25];

	for (int i = 0; i < EYE_PATTERN_COL_SIZE*EYE_PATTERNS_COUNT; ++i)
	{
		completeTemplate[i] = frameTemplate[i];
	}

	Point innerTemplatePosition = Point(1, 1);

	for (int rowOffset = 0; rowOffset < 3; ++rowOffset)
	{
		innerTemplatePosition.row += rowOffset;
		for (int colOffset = 0; colOffset < 3; ++colOffset)
		{
			innerTemplatePosition.column += colOffset;
			int completeTemplateIndex = pointToVectorIndex(innerTemplatePosition, 5);
			int innerTemplateIndex = pointToVectorIndex(Point(rowOffset, colOffset),3);

			completeTemplate[completeTemplateIndex] += internalTemplates[matchedInnerTemplateNum][innerTemplateIndex];
		}
	}

	return completeTemplate;
}


void Solution::scanRegionOfInterest(Image* target, Point regionStart)
{
	int regionDimension = 2 * EYE_PATTERN_COL_SIZE - 1;
	Point regionEnd = Point(regionStart.row + regionDimension, regionStart.column + regionDimension);

	int availableRowShifts = EYE_PATTERN_COL_SIZE;
	int availableColShifts = EYE_PATTERN_COL_SIZE;

	if (regionEnd.row > target->resolution.height)
	{
		int overflowAmount = target->resolution.height - regionEnd.row;
		availableRowShifts -= overflowAmount;
	}

	if (regionEnd.column > target->resolution.width)
	{
		int overflowAmount = target->resolution.width - regionEnd.column;
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
				/*int potentialInternalTemplate = internalTemplateFound(target, subregionStart);

				if (potentialInternalTemplate != -1)
				{
					applyFiltering(target, subregionStart,potentialInternalTemplate);
					markRegionAsProcessed(target, regionStart);
					scanComplete = true;
				}*/
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

	Point currrentPixelPos = Point(regionStart.row, regionStart.column);

	for (int rowOffset = 0; rowOffset < EYE_PATTERN_COL_SIZE; ++rowOffset)
	{
		currrentPixelPos.row += rowOffset;
		for (int colOffset = 0; colOffset < EYE_PATTERN_COL_SIZE; ++colOffset)
		{
			currrentPixelPos.column += colOffset;
			int templateIndex = pointToVectorIndex(Point(rowOffset, colOffset), 5);
			int pixelIndex = pointToVectorIndex(currrentPixelPos, target->resolution.width);


			if (frameTemplate[templateIndex] == 1 && !pixelIsOfInterest(target->pixels[pixelIndex]))
			{
				return false;
			}
			
		}
		currrentPixelPos.column = regionStart.column;
	}
	return true;
}

int Solution::internalTemplateFound(Image* target, Point regionStart)
{
	int internalTemplates[4][9]{
		{0,1,0, 1,0,1, 0,1,0},
		{0,1,0, 0,0,0, 0,1,0},
		{0,0,0, 1,0,1, 0,0,0},
		{1,0,1, 0,0,0, 1,0,1}
	};

	bool matches[4];
	bool mistmatchDetected = false;

	Point currentSubRegionPixel = Point(regionStart.row + 1, regionStart.column + 1);

	for (int templateNumber = 0; templateNumber < 4; ++templateNumber)
	{

		for (int rowOffset = 0; rowOffset < 3; ++rowOffset)
		{
			currentSubRegionPixel.row += rowOffset;
			for (int colOffset = 0; colOffset < 3; ++colOffset)
			{
				currentSubRegionPixel.column += colOffset;
				int templateIndex = pointToVectorIndex(Point(rowOffset, colOffset), 3);
				int pixelIndex = pointToVectorIndex(currentSubRegionPixel, target->resolution.width);
				if (internalTemplates[templateNumber][templateIndex] == 1 && !pixelIsOfInterest(target->pixels[pixelIndex]))
				{
					mistmatchDetected = true;
				}
			}
		}

		if (!mistmatchDetected)
		{
			matches[templateNumber] = true;
		}
	}

	for (int templateNumber = 0; templateNumber < 4; ++templateNumber)
	{
		if (matches[templateNumber])
		{
			return templateNumber;
		}
	}

	return -1;
}

int Solution::pointToVectorIndex(Point point, int32_t imageWidth)
{
	return point.row * imageWidth + point.column;
}

Point Solution::vectroIndexToPoint(int32_t vectorIndex, int32_t imageWidth)
{
	int32_t column = vectorIndex % imageWidth;
	int32_t row = (vectorIndex - column) / imageWidth;
	return Point(row, column);
}
