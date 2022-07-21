// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharComponent.h"
#include "Components/TimeLineComponent.h"
#include "CharCombatComponent.generated.h"

class ECharState;
class AMainChar;
class AMonsterBase;
class APlayerCameraManager;
class AInstantDeathCamera;
class AExecutionCamera;
class AStealthCamera;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharCombatComponent : public UCharComponent
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UCharCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AMainChar* mainChar;
	AMonsterBase* targetMonsterBase;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* hitCurveFloat;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* ultimateCurveFloat;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* executionCameraZoomCurveFloat;

	FTimerHandle staminaPointRecoveryTimer;

	AInstantDeathCamera* instantDeathCamera;

	AExecutionCamera* executionCamera;

	AStealthCamera* stealthCamera;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* recoveryParticle;
	//
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* bloodParticle;

	UParticleSystemComponent* bloodEffect;

	float bloodRightVector;
	float bloodUpVector;
	float bloodPitchRotator;
	float bloodYawRotator;
	//
public:
	bool CheckTargetFront(FVector _vecA, FVector _vecB, FVector _forwardVec);

	void StaminaRecoveryTimer();

	void StaminaPointRecoveryStart(float _delayTime);

	UFUNCTION()
	void TakeDamage(AActor* DamagedActor, float Damage,
			const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	bool CheckAttackAvoid(AActor* _damageCauser);

	bool CheckCharParryingOrDefense(AActor* _damageCauser);

	bool CheckInvincibility();

	bool CheckPlayerDefensed();

	UFUNCTION(BlueprintCallable)
	void AttackSweepTrace();

	void PrintBloodPatricle(AMonsterBase* _target);

	void SpawnBloodParticle(FVector _vec, FRotator _rot);

	void SetAttackBloodDirection(AMonsterBase* _target);

	FORCEINLINE AInstantDeathCamera* GetCharInstantDeathCamera()const { return instantDeathCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AExecutionCamera* GetCharExecutionCamera()const { return executionCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AStealthCamera* GetCharStealthCamera()const { return stealthCamera; }
	
	FORCEINLINE UCurveFloat* GetHitCurveFloat() { return hitCurveFloat; }
	FORCEINLINE UCurveFloat* GetUltimateCurveFloat() { return ultimateCurveFloat; }
	FORCEINLINE UCurveFloat* GetExecutionCameraZoomCurveFloat() { return executionCameraZoomCurveFloat; }
	FORCEINLINE UParticleSystem* GetRecoveryParticle() { return recoveryParticle; }
};
