// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetLockOnWidget.generated.h"

class UImage;

UCLASS(Abstract)
class LSH_API UTargetLockOnWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Offenserse", meta = (AllowPrivateAccess = "true"))
	bool bOffenserseZero;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Stealth", meta = (AllowPrivateAccess = "true"))
	bool bCanStealth;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* targetLockOnImage;

public:
	void SetOffenserseZero(bool _val);
	void SetCanStealth(bool _val);
};
