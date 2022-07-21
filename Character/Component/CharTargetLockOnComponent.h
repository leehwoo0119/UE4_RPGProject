// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharComponent.h"
#include "ECharState.h"
#include "Components/TimeLineComponent.h"
#include "CharTargetLockOnComponent.generated.h"

class ACanTargetingActor;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharTargetLockOnComponent : public UCharComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UCharTargetLockOnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	int maxCanTargetLockOnDis;
	float interpSpeed;
	float cameraPitch;

	ACanTargetingActor* targetMonsterBase;
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* switchCombatModeCameraCurveFloat;

	FTimeline switchCombatModeCameraCurveFTimeline;

	float switchCombatModeCameraCurveLength;

	FVector defaultCameraSocketOffset;

public:
	void TargetLockOn(ACanTargetingActor* _target = nullptr);

	void SwitchTargetLockOn(ETargetSwitchState_Type _switchType);

	TArray<ACanTargetingActor*> CheckTargetLockOn(ACanTargetingActor* _monster = nullptr);

	bool CheckCharCanSeeTarget(ACanTargetingActor* _checkTarget);

	ACanTargetingActor* GetMonsterAtDot(TArray<ACanTargetingActor*> _monsterArr);

	bool CheckTargetFront(FVector _vecA, FVector _vecB, FVector _forwardVec);

	UFUNCTION(BlueprintCallable)
	void ResetTarget(ACanTargetingActor* _target = nullptr);

	TArray<ACanTargetingActor*> TargetSwitch(ETargetSwitchState_Type _switchType);

	bool SetTargetMonster(ACanTargetingActor* _tagetMonster);

	void SetCameraRotationToTarget();

	void SetCharRotationToTarget();

	void SwitchCameraCombatMode(bool _bCombatMode);

	UFUNCTION()
	void SwitchCameraCombatModeStart(float _val);
};
