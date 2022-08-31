#include "Solution.h"
#include "utils/FileParser.h"
#include "utils/Image.h"
#include "utils/SolutionEvaluator.h"
#include <iomanip>
#include "utils/EyePatterns.h"

void writeImageToFile(StrideImage image, int height, int width, std::string fileName)
{
	std::cout << "Writing to file..." << std::endl;
	std::ofstream file;
	file.open(fileName, std::ios::binary | std::ios::out);
	file << width << "=" << height << "D" << std::endl;

	for (int i = 0; i < height * width; ++i)
	{
		file << std::to_string(image.redPixels[i]);
		file << " ";
		file << std::to_string(image.greenPixels[i]);
		file << " ";
		file << std::to_string(image.bluePixels[i]);
		file << " ";
		file << std::to_string(image.alphaPixels[i]);
		file << "-";
	}


	file.close();
}



using Image = PackedImage;

static void debugPrint(const StrideImage& img,
	int32_t startRow, int32_t startCol) {
	for (int32_t col = startCol; col < startCol + EYE_PATTERN_COL_SIZE; ++col) {
		if (col < 10) {
			std::cout << ' ';
		}
		std::cout << "[" << col << "] ";
	}
	std::cout << std::endl;

	for (int32_t row = startRow; row < startRow + EYE_PATTERN_COL_SIZE; ++row) {
		if (row < 10) {
			std::cout << ' ';
		}
		std::cout << "[" << row << "]";

		for (int32_t col = startCol; col < startCol + EYE_PATTERN_COL_SIZE; ++col) {
			const int32_t pixelIdx = (row * img.resolution.width) + col;
			std::cout << std::setw(4) << (int)img.redPixels[pixelIdx] << ' ';
		}
		std::cout << std::endl;
	}
}

int32_t main([[maybe_unused]] int32_t argc, [[maybe_unused]] char* argv[]) {
	//Use the provided implementation that best suits your needs
	//using Image = PackedImage;

	using Image = StrideImage;

	std::vector<Image> inputImages;
	std::vector<Image> outputImages;
	int32_t err = FileParser<Image>::generateData(inputImages, outputImages);
	if (EXIT_SUCCESS != err) {
		std::cerr << "FileParser::generateData() failed" << std::endl;
		return EXIT_FAILURE;

	}

	debugPrint(inputImages[0], 149, 295);

	for (int i = 0; i < inputImages.size(); ++i)
	{
		std::string fileName = "o_small_image" + std::to_string(i)+".txt";
		writeImageToFile(inputImages[i], inputImages[i].resolution.height, inputImages[i].resolution.width, fileName);
	}

	Solution solution;
	//solution.compute(inputImages);

	err = SolutionEvaluator<Image>::compare(inputImages, outputImages);
	if (EXIT_SUCCESS != err) {
		std::cout << "Solution status - [FAIL]" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Solution status - [SUCCESS]" << std::endl;
	return EXIT_SUCCESS;
}



