// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DotDamageActor.generated.h"

class EAIState;
class UParticleSystemComponent;
class AMainChar;
class AMonsterBase;

UCLASS()
class LSH_API ADotDamageActor : public AActor
{
	GENERATED_BODY()

public:
	ADotDamageActor();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UParticleSystemComponent* particleSystemComp;

	AMainChar* mainChar;
	AMonsterBase* monsterBase;

	FTimerHandle flameDotDamageHandle;
	FTimerHandle poisonDotDamageHandle;
	FTimerHandle poisonStackHandle;
	FTimerHandle frozenCrowdControlHandle;
	FTimerHandle frozenStackHandle;

	int flameDamageNum;
	int poisonDamageNum;
	int poisonStack;
	int frozenStack;

	int maxFlameDamageNum;
	int maxPoisonDamageNum;
	int maxPoisonStack;
	int maxFrozenStack;

	int poisonDotTimer;
	int FrozenCCTimer;

public:
	void DotDamageActorDestroy();

	void SetFlameDamageNum(int _value);
	void PlusPoisonStack();
	void PlusFrozenStack();

	void SetPoisonDotTimerNum(int _value);
	void SetFrozenCCTimerNum(int _value);

	FORCEINLINE int GetMaxFlameDamageNum() { return maxFlameDamageNum; }
};
