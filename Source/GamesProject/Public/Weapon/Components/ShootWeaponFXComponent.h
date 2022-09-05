
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShootCoreTypes.h"			//���������� ��� ������� � �������
#include "ShootWeaponFXComponent.generated.h"


class UNiagaraSystem; //������� �����������
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMESPROJECT_API UShootWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UShootWeaponFXComponent();

	void PlayImpactFX(const FHitResult& Hit);	//FHitResult - ��� �������� ���� ������, ��������� � ����, ������� � ��.

protected:

	//������ ������� ����� ����������
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "VFX")
		//�������� ��-�� ���������� ������� UNiagaraSystem* DefaultEffect;			//����������� � ��� ��������� ������� �� ����� ����������
		FImpactData DefaultImpactData;				//��������� ����� ��� ������, ��� � ������� �������

	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "VFX")
		//�������� ��-�� ���������� ������� TMap<UPhysicalMaterial*, UNiagaraSystem*> EffectsMap;//TMap - ������ ������ ��� ���������� ����� + ��������, ��������� ���� ������� ����� � ��� �������. ���� ����� = ��� ��������, �������� = ������� �������
		TMap<UPhysicalMaterial*, FImpactData> ImpactDataMap; //TMap - ������ ������ ��� ���������� ����� + ��������, ��������� ���� ������� ����� � ��� �������.���� ����� = ��� ��������, �������� = ImpactDataMap
};
