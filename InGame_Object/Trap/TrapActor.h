// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapActor.generated.h"

class UBoxComponent;

UENUM(BlueprintType)
enum class ETrapSection : uint8
{
	Default UMETA(DisplayName = "Default"),
	Section1 UMETA(DisplayName = "Section1"),
	Section2 UMETA(DisplayName = "Section2"),
	Section3 UMETA(DisplayName = "Section3")
};

UCLASS()
class LSH_API ATrapActor : public AActor
{
	GENERATED_BODY()

public:
	ATrapActor();

protected:
	virtual void BeginPlay() override;

protected:
	static TArray<ATrapActor*> trapArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trap")
	UBoxComponent* trapCollision;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "TrapSection")
	ETrapSection trapSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool trapActive;

	void AddTrapArray(ATrapActor* _trapActor);

	void RemoveTrapArray(ATrapActor* _trapActor);

	void NotifyTrapDeactive(ETrapSection _trapSection);

	FORCEINLINE ETrapSection GetTrapSection() { return trapSection; }
};
