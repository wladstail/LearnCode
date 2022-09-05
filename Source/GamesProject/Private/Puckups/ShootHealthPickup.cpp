

#include "Puckups/ShootHealthPickup.h"
#include "Components/ShootHealtsComponent.h"	//����������
#include "ShootUtils.h"							//����������

//��� ������
DEFINE_LOG_CATEGORY_STATIC(LogHealthPickup, All, All);



bool AShootHealthPickup::GivePickupTo(APawn* PlayerPawn)
{
	//UE_LOG(LogHealthPickup, Display, TEXT("Pickup Health was taken"));

	//�������� ��������� ��������
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent<UShootHealtsComponent>(PlayerPawn);	//��� ������� � ��������

	//���������  ���������
	if (!HealthComponent) return false;

	//���������� ������ ���� �������� ������� ���� ���������
	return HealthComponent->TryToAddHealth(HealthAmount);
}