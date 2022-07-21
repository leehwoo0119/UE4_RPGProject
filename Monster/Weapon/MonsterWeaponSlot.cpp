// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterWeaponSlot.h"
#include "MonsterWeapon.h"
#include "MonsterBase.h"

// Sets default values for this component's properties
UMonsterWeaponSlot::UMonsterWeaponSlot()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UMonsterWeaponSlot::BeginPlay()
{
	Super::BeginPlay();

	FTransform tr;
	tr.SetRotation(spawnWeaponRot.Quaternion());
	check(monsterFirstWeaponBP != nullptr);	
	monsterFirstWeapon = GetWorld()->SpawnActor<AMonsterWeapon>(monsterFirstWeaponBP, tr);
	
	//두번째 무기는 필수가 아니다
	if (monsterSecondWeaponBP)
		monsterSecondWeapon = GetWorld()->SpawnActor<AMonsterWeapon>(monsterSecondWeaponBP, tr);
}

void UMonsterWeaponSlot::SetMonsterFirstWeaponSocket(FName _socketName)
{
	monsterFirstWeapon->GetMonsterWeaponMesh()->AttachToComponent(monsterBase->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(_socketName));
}

void UMonsterWeaponSlot::SetMonsterSecondWeaponSocket(FName _socketName)
{
	monsterSecondWeapon->GetMonsterWeaponMesh()->AttachToComponent(monsterBase->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName(_socketName));
}

void UMonsterWeaponSlot::SetMonsterBase(AMonsterBase* _monsterBase)
{
	monsterBase = _monsterBase;
	check(monsterBase != nullptr);
}
