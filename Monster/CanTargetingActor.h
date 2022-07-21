// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CanTargetingActor.generated.h"

class UWidgetComponent;

UCLASS()
class LSH_API ACanTargetingActor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACanTargetingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "WidgetComponent", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* targetLockOnWidget;

private:
	bool bAttackOnce;

public:
	FORCEINLINE UWidgetComponent* GetTargetLockOnWidget()const { return targetLockOnWidget; }
	FORCEINLINE bool GetAttackOnce()const { return bAttackOnce; }

	void SetAttackOnce(bool _val);
};
