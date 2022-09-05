// Shoot Game


#include "Puckups/ShootAmmoPickup.h"
#include "Components/ShootHealtsComponent.h"	//����������
#include "Weapon/ShootWeaponComponent.h"		//����������
#include "ShootUtils.h"							//����������

//���� ������
DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

//�������� �������� ��� ������������� ���� ������
bool AShootAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	//�������� ��������� ��������
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent<UShootHealtsComponent>(PlayerPawn);	//��� ������� � ��������
	
	//��������� ��� �� �������� ||(���) ���������� �� ���������
	if (!HealthComponent || HealthComponent->IsDead()) return false;	//������� ��� ��� �������� ��� ������ ������� �� ��

	//�������� ��������� ������
	const auto WeaponComponent = ShootUtils::GetShootPlayerComponent<UShootWeaponComponent>(PlayerPawn);	//��� ������� � ��������

	//��������� ���������� �� ���������
	if (!WeaponComponent) return false;

	//UE_LOG(LogAmmoPickup, Display, TEXT("Pickup Ammo was taken"));
	
	//���� ���������� �������� 
	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);	//�������� ��� ������ � ���������� ��������
}