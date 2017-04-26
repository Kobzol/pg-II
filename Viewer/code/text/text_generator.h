#pragma once

#include <vector>
#include <string>
#include <random>
#include <ctime>

struct TextParticle
{
public:
	char character;
	float alpha;
	glm::vec3 color;
};
struct TextColumn
{
public:
	float y;
	std::vector<TextParticle> particles;
	float speed;
};

class TextGenerator
{
public:
	TextGenerator(int cols, int height, float textHeight)
		: cols(cols), height(height), verticalOffsetDistribution(0, height / 5.0f), textHeight(textHeight)
	{
		for (int i = 0; i < cols; i++)
		{
			this->matrix.push_back({ 0.0f, {}, 0.0f });
		}

		for (int i = 0; i < 3; i++)
		{
			this->generators[i].seed((unsigned int) time(nullptr) + i);
		}

		this->initText();
	}

	void update(float delta)
	{
		for (auto& col : this->matrix)
		{
			col.y += delta * col.speed;
			if (col.y < this->height)
			{
				for (auto& particle : col.particles)
				{
					if (this->flipCharacterDistribution(this->generators[4]) < 1)
					{
						particle.character = this->generateChar();
					}
				}
			}
			else this->spawnColumn(col);
		}
	}

	std::vector<TextColumn> matrix;
	int cols;
	int height;
	float textHeight;

private:
	std::default_random_engine generators[6];
	std::uniform_int_distribution<int> charDistribution{ 48, 126 };
	std::uniform_real_distribution<float> sentenceLengthDistribution{ 15.0f, 45.0f };
	std::uniform_int_distribution<int> spawnColumnDistribution{ 0, 100 };
	std::uniform_real_distribution<float> verticalOffsetDistribution;
	std::uniform_int_distribution<int> flipCharacterDistribution{ 0, 100 };
	std::uniform_real_distribution<float> textSpeedDistribution{ 1.0f, 3.0f };

	void initText()
	{
		for (auto& col : this->matrix)
		{
			this->spawnColumn(col, true);
		}
	}
	void spawnColumn(TextColumn& col, bool randomHeight = false)
	{
		int sentence = std::max(1.0f, this->sentenceLengthDistribution(this->generators[1]));
		float delta = 1.0f / (float) sentence;
		float alpha = 0.25f;

		const glm::vec3 startColor(0.0f, 0.6f, 0.0f);
		const glm::vec3 endColor(1.0f, 1.0f, 1.0f);
		const float endColorRatioStart = 0.65;

		col.particles.clear();

		for (int i = 0; i < sentence; i++)
		{
			TextParticle particle;
			particle.alpha = std::min(1.0f, alpha);
			alpha += (delta * 2.0f);
			particle.character = this->generateChar();

			float ratio = ((float) i) / sentence;

			if (ratio < endColorRatioStart)
			{
				particle.color = startColor;
			}
			else particle.color = glm::mix(startColor, endColor, (ratio - endColorRatioStart) / (1.0f - endColorRatioStart));

			col.particles.push_back(particle);
		}

		col.y = randomHeight ? (this->verticalOffsetDistribution(this->generators[3])) : (-this->textHeight * sentence) * 1.25f;
		col.speed = this->generateSpeed();
	}
	char generateChar()
	{
		return this->charDistribution(this->generators[0]);
	}
	float generateSpeed()
	{
		return this->textSpeedDistribution(this->generators[5]);
	}
};
