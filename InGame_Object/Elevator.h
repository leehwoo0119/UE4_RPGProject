// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimeLineComponent.h"
#include "Elevator.generated.h"


class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class LHW_API AElevator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AElevator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = "true"))
	FVector startVec;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Vector", meta = (AllowPrivateAccess = "true"))
	FVector endVec;

	FVector buttonStartVec;

	FVector buttonEndVec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* buttonPushCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Button", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* elevatorPushButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* elevatorPeace_1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* elevatorPeace_2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* elevatorPeace_3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* elevatorPeace_4;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* elevatorCurveFloat;

	FTimeline elevatorCurveFTimeline;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCurveFloat* elevatorButtonCurveFloat;

	FTimeline elevatorButtonCurveFTimeline;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	float elevatorCurveLength;

	float buttonPushHeight;
	float buttonPushCurveLength;

	FTimerHandle pushButtonHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	float pushButtonDelayTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Elevator", meta = (AllowPrivateAccess = "true"))
	float height;

	USceneComponent* dummyPos;

	bool bElevatorMove;

public:
	void StartElevatorCurveFTimeline();

	UFUNCTION()
	void ElevatorStart(float _val);

	UFUNCTION()
	void ElevatorEnd();

	void StartElevatorButtonCurveFTimeline();

	void ReverseElevatorButtonCurveFTimeline();

	UFUNCTION()
	void ElevatorButtonPushStart(float _val);

	void SetButtonVector();

	UFUNCTION()
	virtual void ElevatorButtonOnOverlapBegin(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
			bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void ElevatorButtonOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
