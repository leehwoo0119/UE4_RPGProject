// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SecondBossAreaPattern.generated.h"

class ASecondBossArea;
class AMonsterBase;
class UNiagaraSystem;
class ASecondBossPatternWeapon;

UCLASS()
class LHW_API ASecondBossAreaPattern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecondBossAreaPattern();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float areaSize;

	TArray<FVector> areaPosArr;
	TArray<ASecondBossArea*> areaArr;
	TMap<float, ASecondBossArea*> dangerousAreaMap;

	TArray<FVector> patternWeaponAreaArr;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ASecondBossArea* safeArea;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AMonsterBase* monster;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* damageParticle;

	int patternCnt;
	int maxPatternCnt;
	int patternWeaponCnt;
	int maxPatternWeaponCnt;
	int damageCnt;

	FTimerHandle spawnTimer;
	FTimerHandle damageTimer;
	FTimerHandle executeDamageTimer;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bLoop;

public:
	UPROPERTY(EditAnyWhere, Category = "Area")
	TSubclassOf<ASecondBossArea> secondBossAreaBP;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BossPatternStart();

	UFUNCTION(BlueprintCallable)
	void CheckSafeArea();

	void CheckDangerousArea();

	UFUNCTION(BlueprintCallable)
	void CheckPatternWeapon();

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnPatternWeapon(FVector _spawnVec);

	UFUNCTION(BlueprintCallable)
	void CheckPatternWeaponCnt(int _val);

	void DestroyPatternArea();

	FORCEINLINE TArray<ASecondBossArea*> GetPatternArea()const { return areaArr; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMaxPatternCnt()const { return maxPatternCnt; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetPatternCnt()const { return patternCnt; }

	UFUNCTION(BlueprintCallable)
	void ResetPatternCnt();

	void ExecuteDamage();

};
