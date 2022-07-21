// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExecutionCamera.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class LHW_API AExecutionCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExecutionCamera();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* executionCamera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* springArm;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UCameraComponent* GetExecutionCamera()const { return executionCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USpringArmComponent* GetSpringArm()const { return springArm; }
};
