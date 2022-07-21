// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimeLineComponent.h"
#include "MainCharBow.generated.h"

class USkeletalMeshComponent;
class UMainCharBowAnimInstance;
class AMainChar;

UCLASS()
class LHW_API AMainCharBow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainCharBow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* bow;

	UMainCharBowAnimInstance* animInstance;

	AMainChar* mainChar;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* chargingDrawCurveFloat;

	FTimeline  chargingCurveFTimeline;

	float chargingCurveLength;

public:
	FORCEINLINE USkeletalMeshComponent* GetBowMesh()const { return bow; }

	void StartCharge();

	UFUNCTION(BlueprintCallable)
	void EndCharge();

	UFUNCTION()
	void ChargingDrawing(float _val);

	UFUNCTION()
	void FullCharging();
};
