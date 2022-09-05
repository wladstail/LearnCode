

#include "Weapon/Components/ShootWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"	//��� ������� ����������
#include "PhysicalMaterials/PhysicalMaterial.h" //��� ����������� ��������� ����������
#include "Kismet/GameplayStatics.h"		//���������� ��� �������
#include "Components/DecalComponent.h"	//���������� ��� �������




UShootWeaponFXComponent::UShootWeaponFXComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

//��������� ������ � �������
void UShootWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{

	//�������� ��-�� ������� auto Effect = DefaultEffect;	//�������� ������� ��������� � ����������. ����� �������� ������������ ���� ������
	auto ImpactData = DefaultImpactData;	//�������� ������� ��������� � ����������. ����� �������� ������������ ���� ������

	//� ��������� �������� ���� � ��� ��������� � ��������� �� �������������
	if (Hit.PhysMaterial.IsValid())
	{
		//���� ������ �� �������� ������ � ������ ���������
		const auto PhysMat = Hit.PhysMaterial.Get();	//�������� ��������� �� ���������� �������� �.�. ���������� ��� ����� �� ��� ��������

		//�������� ���������� �� ������ ��� ���������� ��� ���������
		//�������� ��-�� ������� if (EffectsMap.Contains(PhysMat))	//Contains - ��������� ���������� �� ���� ������� �������� � �������� �������, ������ ����������
		if (ImpactDataMap.Contains(PhysMat))	//Contains - ��������� ���������� �� ���� ������� �������� � �������� �������, ������ ����������
		{
			//���� ����������, �������� ��������������� ��� �������� � �������������� ������ ������� ����� ����������
			//�������� ��-�� ������� Effect = EffectsMap[PhysMat];	//��� ������� �������� �� ����� [], ������ ��������� �� ��� ��������
			ImpactData = ImpactDataMap[PhysMat];	//��� ������� �������� �� ����� [], ������ ��������� �� ��� ��������
		}
	}

	//����� ������� � ���. �������� ��������� ������� � ���� ������, ������� �� ����� 
	//�������� ��-�� �������UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());	//1 ��� 2 ��������� �� ������� �������  3 ������� � ���� (�������� ����� ������) 4 ���������� � ������������
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());	//1 ��� 2 ��������� �� ������� �������  3 ������� � ���� (�������� ����� ������) 4 ���������� � ������������

	//����� ������ � ���		//SpawnDecalAttached - ����� � ������������ � ���������� �� �����.  
	//������� ������ ������ ���������� ��������� �� ��������� ������. ������� ������� ���������� DecalComponent � � ��� �������� ��������� ������� ������, ��� ���� ��� ����� ������
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), //1 ��������� �� ���
		ImpactData.DecalData.Material, //2 �������� ������( � ����� ���������)
		ImpactData.DecalData.Size, //3 ������ ������
		Hit.ImpactPoint, //4. ������� ������ � ���� (��� �� ������ � �������)
		Hit.ImpactNormal.Rotation());	//5 �������� ������. ����� ��� ��� ������ ������� ������� ������������

		//���� �� ������� �� ���������� ������
		if(DecalComponent)
		{
			DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);	//SetFadeOut, SetFadeIn - ��������� ������ 1 ����� ����� ����� ����������� �������� 2 ����� �������� ����������� 3. �������� owner ������, ������������ ���� ������������ SpawnDecalAttached
		}	 
}