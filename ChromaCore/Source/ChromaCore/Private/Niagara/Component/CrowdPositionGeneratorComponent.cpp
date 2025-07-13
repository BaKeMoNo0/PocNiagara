#include "Niagara/Component/CrowdPositionGeneratorComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"


class UNiagaraComponent;

void UCrowdPositionGeneratorComponent::GeneratePositions(int32 SpawnCount, float Spacing, int32 FormType) {
	GeneratedPositions.Empty();

	int32 CountPerAxis = (FormType == 0) ? FMath::CeilToInt(FMath::Pow(SpawnCount, 1.0f / 3.0f)) : FMath::CeilToInt(FMath::Sqrt((float)SpawnCount));

	for (int32 i = 0; i < SpawnCount; ++i) {
		FVector Pos;

		if (FormType == 0) {
			// Cube
			int32 x = i % CountPerAxis;
			int32 y = (i / CountPerAxis) % CountPerAxis;
			int32 z = (i / (CountPerAxis * CountPerAxis));

			float center = (CountPerAxis - 1.0f) * 0.5f * Spacing;
			Pos = FVector(x, y, z) * Spacing - FVector(center);
		} else {
			// Plane
			int32 x = i % CountPerAxis;
			int32 y = i / CountPerAxis;

			float center = (CountPerAxis - 1.0f) * 0.5f * Spacing;
			Pos = FVector(x * Spacing - center, y * Spacing - center, 0.0f);
		}

		GeneratedPositions.Add(Pos);
	}
}

void UCrowdPositionGeneratorComponent::PushToNiagara(UNiagaraComponent* Niagara) {
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
		Niagara, 
		TEXT("TargetPositions"), 
		GeneratedPositions
	);
}

