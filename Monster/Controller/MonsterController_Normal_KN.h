// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterController_Normal.h"
#include "MonsterController_Normal_KN.generated.h"


UCLASS()
class LSH_API AMonsterController_Normal_KN : public AMonsterController_Normal
{
	GENERATED_BODY()
	
public:
	AMonsterController_Normal_KN();

protected:
	virtual void BeginPlay() override;

public:
	void PlayerAttackDetected()override;
	bool SpecialSkill()override;
};
