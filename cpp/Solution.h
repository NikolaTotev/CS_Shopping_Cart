#pragma once
#include <vector>

#include "utils/Image.h"

class Solution
{
	using Image = StrideImage;
public:
	void compute(std::vector<Image> inputImages);
};
