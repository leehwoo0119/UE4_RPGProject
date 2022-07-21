// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialMissionList.generated.h"

class ATutorialManager;
class UImage;

UCLASS()
class LSH_API UTutorialMissionList : public UUserWidget
{
	GENERATED_BODY()

private:
	ATutorialManager* tutorialManager;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* missionUIImage;
	UPROPERTY(meta = (BindWidget))
	UImage* checkImage_1;
	UPROPERTY(meta = (BindWidget))
	UImage* checkImage_2;
	UPROPERTY(meta = (BindWidget))
	UImage* checkImage_3;
	UPROPERTY(meta = (BindWidget))
	UImage* checkImage_4;

	UFUNCTION(BlueprintCallable)
	void SetTutorialManager(ATutorialManager* _tutorialManager);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE ATutorialManager* GetTutorialManager() { return tutorialManager; }
};
