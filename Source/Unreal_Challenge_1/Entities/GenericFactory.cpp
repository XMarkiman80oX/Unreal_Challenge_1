// Fill out your copyright notice in the Description page of Project Settings.


#include "GenericFactory.h"

AGenericFactory::AGenericFactory()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    this->Mesh_Component = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Renderer"));
    this->Mesh_Component->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AGenericFactory::BeginPlay()
{
    Super::BeginPlay();
    switch (this->Factory_Type) {
        case EFactoryType::LUMBERJACKS_HUT:
            this->Production_Rate = 5.0f;
            this->FactoryName = "Lumberjack's Hut";
            break;
        case EFactoryType::IRON_MINE:
            this->FactoryName = "Iron Mine";
            break;
        case EFactoryType::COAL_MINE:
            this->FactoryName = "Coal Mine";
            break;
        case EFactoryType::SEWING_MACHINE_FACTORY:
            this->FactoryName = "Sewing Machine Factory";
            break;
        case EFactoryType::FURNACE:
            this->FactoryName = "Furnace";
            break;
        default:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Factory Type Was Set");
            this->FactoryName = "NO NAME";
            break;
    }
    /*switch (this->Factory_Type) {
        case EFactoryType::FURNACE:
            this->Production_Rate = 2.5f;
            break;
        case EFactoryType::SEWING_MACHINE_FACTORY:
            this->Production_Rate = 10.0f;
            break;
        default:
            this->Production_Rate = 5.0f;
            break;
    }*/
   /* if(this->FactoryStats)
        this->FactoryStats->SetAttachedFactory(this);*/
    GetWorld()->GetTimerManager().SetTimer(this->ProductionTimerHandle, this, &AGenericFactory::ProduceMaterial, this->Production_Rate, true);
}

// Called every frame
void AGenericFactory::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    this->CalculateProgress();
}

void AGenericFactory::ProduceMaterial()
{
    if (Max_Num_Of_Materials > this->Produced_Materials.Num())
    {
        //this->ProductionHasBegun = true;
        AFactoryMaterial* Produced_Material = GetWorld()->SpawnActor<AFactoryMaterial>(AFactoryMaterial::StaticClass());
        this->Produced_Materials.Add(Produced_Material);

        switch (this->Factory_Type) {
            case EFactoryType::LUMBERJACKS_HUT:
                Produced_Material->SetMaterialType(EMaterialType::LUMBER);
                break;
            case EFactoryType::IRON_MINE:
                Produced_Material->SetMaterialType(EMaterialType::IRON);
                break;
            case EFactoryType::COAL_MINE:
                Produced_Material->SetMaterialType(EMaterialType::COAL);
                break;
            default:
                GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Factory Type Was Set");
                break;
        }

        if (Produced_Material)
        {
            Produced_Material->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        }
        this->StartTime = GetWorld()->GetTimeSeconds();
        //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "This Runs");
    }
}

TMap<EFactoryType, int> AGenericFactory::SendMaterials()
{
    //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Materials Were Sent");
    TMap<EFactoryType, int> MaterialsToSend;

    MaterialsToSend.Add(this->Factory_Type, this->Produced_Materials.Num());

    for (AFactoryMaterial* Material : this->Produced_Materials)
    {
        // Destroy the child actor
        Material->Destroy();
    }
    this->Produced_Materials.Empty();

    return MaterialsToSend;
}

void AGenericFactory::CalculateProgress()
{
    this->Time_Elapsed = GetWorld()->GetTimeSeconds() - this->StartTime;
    this->Time_Elapsed_Float = this->Time_Elapsed / this->Production_Rate;
    if (this->Time_Elapsed_Float > 1.0f)
        this->Time_Elapsed_Float = 1.0f;

    this->Time_Elapsed_Percentage = Time_Elapsed_Float * 100;
    this->Time_Elapsed_Percentage = FMath::CeilToInt(this->Time_Elapsed_Percentage);
    this->MaterialsCount = this->Produced_Materials.Num();
}

EFactoryType AGenericFactory::GetFactoryType()
{
    return this->Factory_Type;
}

int AGenericFactory::GetProducedMaterialsCount()
{
    return this->MaterialsCount;
}

FString AGenericFactory::GetFactoryName()
{
    switch (this->Factory_Type) {
        case EFactoryType::LUMBERJACKS_HUT:
            return "Lumberjack's Hut";
            break;
        case EFactoryType::IRON_MINE:
            return "Iron Mine";
            break;
        case EFactoryType::COAL_MINE:
            return "Coal Mine";
            break;
        case EFactoryType::SEWING_MACHINE_FACTORY:
            return "Sewing Machine Factory";
            break;
        case EFactoryType::FURNACE:
            return "Furnace";
            break;
        default:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Factory Type Was Set");
            return "NO NAME";
            break;
    }

}

