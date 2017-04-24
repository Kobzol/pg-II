#pragma once

#include <vector>
#include <string>
#include <random>

static std::default_random_engine generator;
static std::uniform_int_distribution<int> charDistribution(32, 126);
static std::uniform_int_distribution<int> spaceDistribution(0, 100);
static float spaceChance = 0.4f;

class TextGenerator
{
public:
	TextGenerator(int rows, int cols) : rows(rows), cols(cols)
	{
		for (int i = 0; i < cols; i++)
		{
			this->matrix.push_back(std::string(rows, 'a'));
		}

		this->permute();
	}

	void permute()
	{
		for (auto& col : this->matrix)
		{
			for (size_t i = 0; i < col.size(); i++)
			{
				bool space = spaceDistribution(generator) < (spaceChance * 100.0f);
				char c = space ? ' ' : (charDistribution(generator));
				col[i] = c;
			}
		}
	}

	void move()
	{
		for (size_t i = 0; i < this->matrix.size(); i++)
		{
			size_t length = this->matrix[i].size();
			for (size_t j = 0; j < length; j++)
			{
				std::swap(this->matrix[i][length - 1], this->matrix[i][j]);
			}
		}
	}

	std::vector<std::string> matrix;
	int rows, cols;
};
