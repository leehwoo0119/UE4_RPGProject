// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class LHW_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ItemID", meta = (AllowPrivateAccess = "true"))
	int itemID;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CloseDoor();

	FORCEINLINE int GetItemID()const { return itemID; }
};
