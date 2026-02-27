#pragma once
#include "src/system/Observer.h"

struct ScoreData
{
	int score;
	int mistakes;
	int maxMistakes;
};

class ScoreModel final : public CObservable<ScoreData>
{
public:
	ScoreModel();

	void AddWin();
	bool AddMistakeAndCheckLoss();
	void Reset();

	const ScoreData& GetData() const;

protected:
	ScoreData GetChangedData() const override;

private:
	ScoreData m_data;
};