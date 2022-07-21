// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase_Boss.h"
#include "MonsterBase_Boss_First.generated.h"

class UParticleSystemComponent;

UCLASS()
class LSH_API AMonsterBase_Boss_First : public AMonsterBase_Boss
{
	GENERATED_BODY()

public:
	AMonsterBase_Boss_First();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeParticle", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* eyeParticle_r;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EyeParticle", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* eyeParticle_l;

public:
	UFUNCTION(BlueprintCallable)
	void SetEyeParticle(bool _bActive);
};
