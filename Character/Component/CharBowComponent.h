// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharComponent.h"
#include "Components/TimeLineComponent.h"
#include "CharBowComponent.generated.h"

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharBowComponent : public UCharComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharBowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float bowMaxDis;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* bowChargingCameraCurveFloat;

	FTimeline  bowChargingCameraCurveFTimeline;

	float bowChargingCameraCurveLength;

	FVector charSprintArmSocketVec;

	float charSpringArmLength;

public:
	void StartBowChargingCameraCurveFTimeline();

	UFUNCTION(BlueprintCallable)
	void ReverseBowChargingCameraCurveFTimeline();

	void SetArrowShootDir(bool _bShooting = false);

	UFUNCTION()
	void BowChargingCameraZoomStart(float _val);
};
