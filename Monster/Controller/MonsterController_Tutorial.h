// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterController_Normal.h"
#include "MonsterController_Tutorial.generated.h"

/**
 * 
 */
UCLASS()
class LSH_API AMonsterController_Tutorial : public AMonsterController_Normal
{
	GENERATED_BODY()
	
public:
	AMonsterController_Tutorial();

protected:
	virtual void BeginPlay() override;
};
