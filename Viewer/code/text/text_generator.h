#pragma once

#include <vector>
#include <string>
#include <random>
#include <ctime>

static std::default_random_engine generator;
static std::uniform_int_distribution<int> charDistribution(32, 126);
static std::uniform_int_distribution<int> spaceDistribution(0, 100);
static std::normal_distribution<float> sentenceLength(4.0f, 6.0f);
static std::normal_distribution<float> spaceLength(3.0f, 1.0f);
static float spaceChance = 0.4f;

struct TextParticle
{
public:
	char character;
	float life;
};

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
		generator.seed((unsigned int) time(nullptr));

		for (auto& col : this->matrix)
		{
			size_t i = 0;
			while (i < col.size())
			{
				int sentence = std::max(1.0f, sentenceLength(generator));
				while (i < col.size() && sentence--)
				{
					col[i++] = charDistribution(generator);
				}
				int space = std::max(1.0f, spaceLength(generator));
				while (i < col.size() && space--)
				{
					col[i++] = ' ';
				}
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
