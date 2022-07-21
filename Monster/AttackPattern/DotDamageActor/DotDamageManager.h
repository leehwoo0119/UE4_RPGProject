// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EAIState.h"
#include "DotDamageManager.generated.h"

class ADotDamageActor;
class AMonsterBase;
class AMainChar;
class AMonsterProjectile;
class AMonsterElementalOrb;

UCLASS()
class LSH_API ADotDamageManager : public AActor
{
    GENERATED_BODY()

public:
    ADotDamageManager();

protected:
    virtual void BeginPlay() override;

private:
    AMainChar* mainChar;

    UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
       TSubclassOf<ADotDamageActor> DotDamageActor_Flame;
    UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
       TSubclassOf<ADotDamageActor> DotDamageActor_Poison;
    UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
       TSubclassOf<ADotDamageActor> DotDamageActor_Frozen;

    UPROPERTY()
        TArray<ADotDamageActor*> dotDamageActorArray;
    UPROPERTY()
        TArray<AMonsterProjectile*> projectileArray;
    UPROPERTY()
        TArray<AMonsterElementalOrb*> elementalOrbArray;

public:
    void CreateParticleActor(AActor* _target, float _targetMaxHealth, FVector const& _location, FRotator const& _rotation,
        AMonsterBase* _monsterBase, EAI_ElementalType_State _elementalType, const FActorSpawnParameters& _spawnParameters = FActorSpawnParameters());

    void AddDotDamageActorArray(ADotDamageActor* _actor);
    void AddProjectileArray(AMonsterProjectile* _actor);
    void AddElementalOrbArray(AMonsterElementalOrb* _actor);

    void AllDotDamageActorDestroy();
    void AllProjectileDestroy();
    void AllElementalOrbDestroy();

    UFUNCTION(BlueprintCallable)
        void AllActorDestroy();

    void PersonalActorDestroy(AMonsterBase* _monsterBase);
};