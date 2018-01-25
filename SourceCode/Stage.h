#pragma once

#include "../CWE/CWELib.h"
#include "BattleEnums.h"
#include "Information.h"



// �X�e�[�W
class Stage
{
private:
	CWE::Polygon mModel;
	CWE::Polygon mSkyDome;

	CWE::math::Float4x4 mLocal;

	bool AllResourceLoadResult(CWE::Task equal) const;

public:
	// �X�e�[�W�̕������
	static const float smGravityAcceleration;
	static const float smGroundFriction;

	Stage(BattleStage stage);
	~Stage();

	const CWE::math::Float4x4& Local() const {return mLocal;}

	bool LoadSuccess() const;
	void Update();
	void DrawStage(Information& info) const;
	void DrawSkyDome(Information& info) const;

};