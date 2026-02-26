#pragma once
#include "GameTypes.h"
#include "src/system/Observer.h"
#include <unordered_map>

struct KeyboardData
{
	std::unordered_map<char, LetterStatus> keys;
};

class KeyboardModel final : public CObservable<KeyboardData>
{
public:
	KeyboardModel();

	void Reset();
	void SetLetterStatus(char letter, LetterStatus status);
	LetterStatus GetLetterStatus(char letter) const;
	const KeyboardData& GetData() const;

protected:
	KeyboardData GetChangedData() const override;

private:
	KeyboardData m_data;

	void InitKeys();
};