// Fill out your copyright notice in the Description page of Project Settings.


#include "World/XarroulInstancedStaticMeshActor.h"
#include "Components/InstancedStaticMeshComponent.h"

AXarroulInstancedStaticMeshActor::AXarroulInstancedStaticMeshActor()
{
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	InstancedStaticMeshCube = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshCube"));
	InstancedStaticMeshCube->SetupAttachment(RootComponent);
	InstancedStaticMeshCube->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InstancedStaticMeshCube->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InstancedStaticMeshOcta = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshOcta"));
	InstancedStaticMeshOcta->SetupAttachment(RootComponent);
	InstancedStaticMeshOcta->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InstancedStaticMeshOcta->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	InstancedStaticMeshTetra = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedMeshTetra"));
	InstancedStaticMeshTetra->SetupAttachment(RootComponent);
	InstancedStaticMeshTetra->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InstancedStaticMeshTetra->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AXarroulInstancedStaticMeshActor::BeginPlay()
{
	Super::BeginPlay();

	//SetupBackgroundMeshs(); - called in editor
}

void AXarroulInstancedStaticMeshActor::SetupBackgroundMeshs()
{
	float FieldSizeDelta = 0.0f;	
	
	for (int i = 0; i < MeshData.Num(); i++)
	{
		FVector OffsetDistance = FVector(1.0f, 1.0f, 0.0f) * (MeshData[i].InstanceSpacing * (MeshData[i].InstanceCountXY / 2.0f));
			
		for (int x = 1; x < MeshData[i].InstanceCountXY + 1; x++)
		{
			for (int y = 1; y < MeshData[i].InstanceCountXY + 1; y++)
			{
				const int Rand = FMath::RandRange(0, 2);

				UInstancedStaticMeshComponent* ISMComp = InstancedStaticMeshCube;
				
				switch (Rand)
				{
					case 0:
						//bump
						break;
					case 1:
						ISMComp = InstancedStaticMeshOcta;
						break;
					case 2:
						ISMComp = InstancedStaticMeshTetra;
						break;
					default: ;
				}

				FTransform InstTransform;

				const FVector Scale = FVector::OneVector * FMath::RandRange(MeshData[i].MinScaleMultiplier, MeshData[i].MaxScaleMultiplier);
				const FVector RandOffset = FVector(FMath::RandRange(-150.0f, 150.0f), FMath::RandRange(-150.0f, 150.0f), 0.0f) * Scale;

				InstTransform.SetScale3D(Scale);

				const float Dist = MeshData[i].InstanceSpacing;

				FVector NewLoc = FVector((Dist * x) + RandOffset.X, (Dist * y) + RandOffset.Y,-MeshData[i].DistanceZ);

				InstTransform.SetLocation(NewLoc - OffsetDistance);
				
				FQuat NewRot = FQuat::Identity;
				NewRot.X = FMath::RandRange(-90.0f, 90.0f);
				NewRot.Y = FMath::RandRange(-90.0f, 90.0f);
				NewRot.Z = FMath::RandRange(-90.0f, 90.0f);
				InstTransform.SetRotation(NewRot.GetNormalized());

				
				ISMComp->AddInstance(InstTransform);

				if(i == 0 && x == 1)
				{
					FieldSizeDelta += Dist;
				}
			}
		}
	}

}

void AXarroulInstancedStaticMeshActor::ClearAllBackgroundMeshs()
{
	InstancedStaticMeshCube->ClearInstances();
	InstancedStaticMeshOcta->ClearInstances();
	InstancedStaticMeshTetra->ClearInstances();

	SetActorLocation(FVector(0.0f, 0.0f, BackgroundZOffset));
}

