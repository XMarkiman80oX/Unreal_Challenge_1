// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UENUM(BlueprintType)
enum class ECheckpointType : uint8
{
	LEFT_CHECKPOINT,
	RIGHT_CHECKPOINT
};

UCLASS()
class UNREAL_CHALLENGE_1_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	//UPROPERTY(EditAnywhere) USceneComponent* Mesh_Component = NULL;
	UPROPERTY(EditAnywhere) ECheckpointType CheckpointType;

public:
	ECheckpointType GetCheckpointType();
};
