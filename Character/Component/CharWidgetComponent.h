// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharWidgetComponent.generated.h"

class UArrowCheckWidget;
class UMainHUD;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LHW_API UCharWidgetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharWidgetComponent();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	UArrowCheckWidget* arrowCheckWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	UMainHUD* mainHUD;

public:
	FORCEINLINE UArrowCheckWidget* GetArrowCheckWidget()const { return arrowCheckWidget; }
	FORCEINLINE UMainHUD* GetMainHUD()const { return mainHUD; }

	UFUNCTION(BlueprintImplementableEvent)
	void CheckShootArrowTarget(bool _bDraw);
};
