#pragma once
#include "GameTypes.h"
#include "src/system/Observer.h"
#include <string>

struct WordData
{
	Riddle riddle;
	std::string maskedWord;
	bool isHintRevealed;
};

class WordModel final : public CObservable<WordData>
{
public:
	WordModel();

	void SetNewRiddle(const Riddle& riddle);
	bool OpenLetter(char letter);
	void RevealHint();

	const std::string& GetMaskedWord() const;
	const std::string& GetDescription() const;
	const std::string& GetHint() const;
	bool IsHintRevealed() const;
	bool IsFullyGuessed() const;

	const WordData& GetData() const;

protected:
	WordData GetChangedData() const override;

private:
	WordData m_data;

	void UpdateMask();
};