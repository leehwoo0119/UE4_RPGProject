// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

class UCanvasPanel;
class UVerticalBox;
class UHorizontalBox;
class USizeBox;

UCLASS()
class LHW_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* mainHUDCanvas;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* charSkillBarBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USizeBox* charArrowCheckImageBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	USizeBox* bossPointBarBox;
};
