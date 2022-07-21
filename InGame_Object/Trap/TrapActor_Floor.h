// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TrapActor.h"
#include "Components/TimeLineComponent.h"
#include "TrapActor_Floor.generated.h"

class UStaticMeshComponent;
class ATrapActor;
class UProjectGameInstance;
class USoundBase;

UCLASS()
class LSH_API ATrapActor_Floor : public ATrapActor
{
	GENERATED_BODY()

public:
	ATrapActor_Floor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* floorMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Trap", meta = (AllowPrivateAccess = "true"))
	USceneComponent* shootPos;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATrapActor> trapProjectile;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* floorDrawCurveFloat;

	FTimeline  floorCurveFTimeline;

	float floorCurveLength;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int makeProjectileNum;

	int projectileNum;

	FTimerHandle fireHandle;

	FVector startVec; 
	FVector fireLoc;
	FRotator fireRot;

	UProjectGameInstance* gameIS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	USoundBase* arrowHitSound;

public:
	UFUNCTION()
	virtual void TrapCollisionOnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void PressDrawing(float _value);

	UFUNCTION()
	void PressOver();

	void Fire();
};
