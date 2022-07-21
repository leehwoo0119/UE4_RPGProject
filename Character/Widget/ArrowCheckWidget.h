// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArrowCheckWidget.generated.h"

class UImage;

UCLASS()
class LHW_API UArrowCheckWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ArrowCheck", meta = (AllowPrivateAccess = "true"))
	bool bCheckTarget;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* arrowCheckImage;

public:
	void SetCheckTarget(bool _val);
};
