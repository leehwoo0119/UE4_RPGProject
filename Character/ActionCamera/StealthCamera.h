// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StealthCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LHW_API AStealthCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStealthCamera();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* stealthCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCameraComponent* GetStealthCamera()const { return stealthCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USpringArmComponent* GetSpringArm()const { return springArm; }
};
