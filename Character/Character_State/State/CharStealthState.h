// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharState.h"
#include "Components/TimeLineComponent.h"
#include "CharStealthState.generated.h"

class AMonsterBase;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharStealthState : public UCharState
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UCharStealthState();

protected:
	// Called when the game starts
	void BeginPlay() override;
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	FTimeline stealth_1_CameraZoomCurveFTimeline;
	FTimeline stealth_2_CameraZoomCurveFTimeline;
	FTimeline stealth_3_CameraZoomCurveFTimeline;
	float stealthCameraZoomCurveLength;

	float minStealth_1_CameraSpringOffset;
	float maxStealth_1_CameraSpringOffset;

	float minStealth_2_CameraSpringLength;
	float maxStealth_2_CameraSpringLength;

	AMonsterBase* targetMonster;

protected:
	void Enter()override;

	void Execute()override;

	void Exit()override;

public:
	void SetCharPos();

	UFUNCTION(BlueprintCallable)
	void StartStealth_1_cameraZoomCurveFTimeline();

	UFUNCTION(BlueprintCallable)
	void ReverseStealth_1_cameraZoomCurveFTimeline();

	UFUNCTION(BlueprintCallable)
	void StartStealth_2_cameraZoomCurveFTimeline();

	UFUNCTION(BlueprintCallable)
	void ReverseStealth_2_cameraZoomCurveFTimeline();

	UFUNCTION(BlueprintCallable)
	void StartStealth_3_cameraZoomCurveFTimeline();

	UFUNCTION(BlueprintCallable)
	void ReverseStealth_3_cameraZoomCurveFTimeline();

	UFUNCTION()
	void Stealth_1_cameraZoomStart(float _val);

	UFUNCTION()
	void Stealth_2_cameraZoomStart(float _val);

	UFUNCTION()
	void Stealth_3_cameraZoomStart(float _val);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AMonsterBase* GetTargetMonster()const { return targetMonster; }
};
