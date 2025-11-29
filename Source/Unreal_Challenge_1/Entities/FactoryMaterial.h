// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FactoryMaterial.generated.h"

UENUM(BlueprintType)
enum class EMaterialType : uint8 {
    IRON,
    STEEL_BEAM,
    LUMBER,
    COAL
    ,SEWING_MACHINE
};

UCLASS()
class UNREAL_CHALLENGE_1_API AFactoryMaterial : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    AFactoryMaterial();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere) EMaterialType Material_Type;

public:
//    void SetMaterialName(FName MaterialName);
    EMaterialType GetMaterialType();
    void SetMaterialType(EMaterialType Material_Type_Param);
};
