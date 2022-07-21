// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstantDeathCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LHW_API AInstantDeathCamera : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AInstantDeathCamera();

private:
	UCameraComponent* ultimateAttackCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm;

public:
	FORCEINLINE UCameraComponent* GetUltimateAttackCamera()const { return ultimateAttackCamera; }
};
