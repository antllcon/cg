#pragma once
#include <string>

enum class LetterStatus
{
	AVAILABLE,
	GUESSED_CORRECTLY,
	GUESSED_INCORRECTLY
};

enum class GameState
{
	PLAYING,
	WORD_GUESSED,
	GAME_OVER
};

struct Riddle
{
	std::string word;
	std::string description;
	std::string hint;
};