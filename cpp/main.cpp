#include "Solution.h"
#include "utils/FileParser.h"
#include "utils/Image.h"
#include "utils/SolutionEvaluator.h"
#include <iomanip>
#include "utils/EyePatterns.h"

void writeImageToFile(PackedImage image, int height, int width, std::string fileName)
{
	std::ofstream file;
	file.open(fileName, std::ios::binary | std::ios::out);
	file << width << "=" << height << "D" << std::endl;

	for (int i = 0; i < height * width; ++i)
	{
		file << std::to_string(image.pixels[i].red);
		file << " ";
		file << std::to_string(image.pixels[i].green);
		file << " ";
		file << std::to_string(image.pixels[i].blue);
		file << " ";
		file << std::to_string(image.pixels[i].alpha);
		file << "-";
	}


	file.close();
}

void saveImages(std::vector<PackedImage> images, std::string namePrefix, std::string saveDirectory)
{
	for (int i = 0; i < images.size(); ++i)
	{
		std::string fileName = namePrefix + "_" + std::to_string(i) + ".rgba";
		std::string savePath = saveDirectory + "\\" + fileName;

		std::cout << "Writing to file [ " << i + 1 << " / " << images.size() << " ]" << "\r";

		writeImageToFile(images[i], images[i].resolution.height, images[i].resolution.width, savePath);
	}
	std::cout << std::endl;
}



using Image = PackedImage;


int32_t main([[maybe_unused]] int32_t argc, [[maybe_unused]] char* argv[]) {

	std::string inputFile;
	std::string outputFile;

	std::cout << "Input file location:" << std::endl;
	std::getline(std::cin, inputFile);

	std::cout << "Target output file location:" << std::endl;
	std::getline(std::cin, outputFile);



	std::string imageSaveSetting;

	std::cout << "Do you want to save before and after images for debugging? [Y/N]" << std::endl;
	std::getline(std::cin, imageSaveSetting);

	bool shouldSaveImages = imageSaveSetting == "Y";
	std::string savePath;

	if (shouldSaveImages)
	{
		std::cout << "Where should the images be saved?" << std::endl;
		std::getline(std::cin, savePath);
	}


	using Image = PackedImage;

	std::vector<Image> inputImages;
	std::vector<Image> outputImages;

	std::cout << "Generating data..." << std::endl;

	int32_t err = FileParser<Image>::generateData(inputImages, outputImages, inputFile, outputFile);

	if (EXIT_SUCCESS != err) {
		std::cerr << "FileParser::generateData() failed" << std::endl;
		return EXIT_FAILURE;
	}

	if (shouldSaveImages)
	{
		std::cout << "Saving original images..." << std::endl;
		saveImages(inputImages, "before_Processing", savePath);
	}

	std::cout << "Computing..." << std::endl;
	Solution solution;
	solution.compute(inputImages);

	std::cout << "Comparing results to target output..." << std::endl;
	err = SolutionEvaluator<Image>::compare(inputImages, outputImages);

	if (EXIT_SUCCESS != err) {

		std::cout << "Solution status - [FAIL]" << std::endl;
		saveImages(inputImages, "o_big_after_Processing", savePath);

		return EXIT_FAILURE;
	}

	std::cout << "Solution status - [SUCCESS]" << std::endl;

	if (shouldSaveImages)
	{
		std::cout << "Saving processed images..." << std::endl;
		saveImages(inputImages, "after_Processing", savePath);
	}


	return EXIT_SUCCESS;
}



