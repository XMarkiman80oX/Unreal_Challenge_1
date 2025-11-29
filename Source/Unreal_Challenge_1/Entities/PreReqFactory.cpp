// Fill out your copyright notice in the Description page of Project Settings.


#include "PreReqFactory.h"

APreReqFactory::APreReqFactory()
{
}
void APreReqFactory::BeginPlay()
{
    Super::BeginPlay();

    switch (this->Factory_Type) {
        case EFactoryType::FURNACE:
            this->Required_Materials.Add(EMaterialType::COAL);
            this->Required_Materials.Add(EMaterialType::IRON);
            this->Production_Rate = 5.5f;
            break;
        case EFactoryType::SEWING_MACHINE_FACTORY:
            this->Required_Materials.Add(EMaterialType::LUMBER);
            this->Required_Materials.Add(EMaterialType::STEEL_BEAM);
            this->Max_Num_Of_Materials = 99;
            this->Production_Rate = 2.0f;
            break;
        default:
            this->Production_Rate = 5.0f;
            break;
    }

    GetWorld()->GetTimerManager().SetTimer(this->ProductionTimerHandle, this, &APreReqFactory::ProduceMaterial, this->Production_Rate, true);
}
void APreReqFactory::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    this->CalculateProgress();
    this->CountInputMaterials();
}
void APreReqFactory::ProduceMaterial()
{
    if (Max_Num_Of_Materials > this->Produced_Materials.Num() && this->AreMaterialsComplete())
    {
        this->InitializeMaterial();
        this->SpendMaterial();
        /*GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "This Runs");
        if (AreMaterialsComplete()) {
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Materials Are Complete");
        }
        else
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Materials Are NOT Complete");*/
        this->StartTime = GetWorld()->GetTimeSeconds();
    }
}

bool APreReqFactory::AreMaterialsComplete()
{
    TArray<EMaterialType> requiredMaterialsHolder = this->Required_Materials;

    for (int i = 0; i < this->Received_Materials.Num() && !requiredMaterialsHolder.IsEmpty(); i++) {
        for (EMaterialType MaterialType : requiredMaterialsHolder) {
            if (this->Received_Materials[i]->GetMaterialType() == MaterialType) {
                requiredMaterialsHolder.RemoveAt(0);
                break;
            }
        }
    }

    if (requiredMaterialsHolder.IsEmpty())
        return true;
    else
        return false;
}

AFactoryMaterial* APreReqFactory::InitializeMaterial()
{
    AFactoryMaterial* Produced_Material = GetWorld()->SpawnActor<AFactoryMaterial>(AFactoryMaterial::StaticClass());
    this->Produced_Materials.Add(Produced_Material);

    switch (this->Factory_Type) {
        case EFactoryType::SEWING_MACHINE_FACTORY:
            Produced_Material->SetMaterialType(EMaterialType::SEWING_MACHINE);
            break;
        case EFactoryType::FURNACE:
            Produced_Material->SetMaterialType(EMaterialType::STEEL_BEAM);
            break;
        default:
            GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "No Factory Type Was Set");
            break;
    }

    if (Produced_Material)
    {
        Produced_Material->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
    }

    return Produced_Material;
}

void APreReqFactory::SpendMaterial()
{
    for (EMaterialType MaterialType : this->Required_Materials)
    {
        for (AFactoryMaterial* InputMaterial : this->Received_Materials)
        {
            if (InputMaterial->GetMaterialType() == MaterialType) 
            {
                this->Received_Materials.Remove(InputMaterial);
                break;
            }
        }
    }
}

void APreReqFactory::CountInputMaterials()
{
    /*this->NumberOfReq1 = 0;
    this->NumberOfReq2 = 0;*/
    int numberOfReq1 = 0;
    int numberOfReq2 = 0;

    for (AFactoryMaterial* InputMaterial : this->Received_Materials)
    {
        if (this->Required_Materials[0] == InputMaterial->GetMaterialType())
        {
            numberOfReq1++;
        }
        if (this->Required_Materials[1] == InputMaterial->GetMaterialType())
        {
            numberOfReq2++;
        }
    }

    this->NumberOfReq1 = numberOfReq1;
    this->NumberOfReq2 = numberOfReq2;
}

void APreReqFactory::ReceiveMaterials(TMap<EMaterialType, int> ReceivedMaterials)
{
    AFactoryMaterial* Received_Material = NULL;
    int* MaterialCount;

    for (EMaterialType RequiredMaterialType : this->Required_Materials) 
    {
        ReceivedMaterials.Find(RequiredMaterialType);
        MaterialCount = ReceivedMaterials.Find(RequiredMaterialType);

        for (int i = 0; i < *MaterialCount; i++)
        {
            Received_Material = GetWorld()->SpawnActor<AFactoryMaterial>(AFactoryMaterial::StaticClass());
            this->Received_Materials.Add(Received_Material);
            Received_Material->SetMaterialType(RequiredMaterialType);
            Received_Material->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
}

TArray<EMaterialType> APreReqFactory::GetRequiredMaterials()
{
    return this->Required_Materials;
}
