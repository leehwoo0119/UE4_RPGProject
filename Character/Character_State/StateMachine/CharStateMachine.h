// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharStateMachine.generated.h"

class UCharState;
class ACharStateActor;
class AMainChar;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharStateMachine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
		
public:
	UCharState* curCharState;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "CharStateActor")
	ACharStateActor* charStateActor;

	UPROPERTY(EditAnyWhere, Category = "CharStateActor")
	TSubclassOf<ACharStateActor> charStateActorBP;

public:
	void SetCurState(UCharState* _state);

	UFUNCTION(BlueprintCallable)
	void SetCharState(UCharState* _state);

private:
	void ChangeState(UCharState* _state);
	void StateUpdate();
};
