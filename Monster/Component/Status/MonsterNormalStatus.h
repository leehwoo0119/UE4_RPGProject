// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBaseStatus.h"
#include "MonsterNormalStatus.generated.h"


UCLASS()
class LSH_API UMonsterNormalStatus : public UMonsterBaseStatus
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMonsterNormalStatus();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	int defenseCnt;

	bool bInvestigateMoveStart;

	EAI_ElementalType_State aiElementalTypeState;

public:
	FORCEINLINE int GetDefenseCnt() const override{ return defenseCnt; }
	FORCEINLINE bool GetInvestigateMoveStart()const override { return bInvestigateMoveStart; }
	FORCEINLINE EAI_ElementalType_State GetElementalType() const override { return aiElementalTypeState; }
	
	void SetDefenseCnt(int _cnt)override;
	void SetInvestigateMoveStart(bool _val)override;
	void SetElementalType(EAI_ElementalType_State _elemental)override;
};
