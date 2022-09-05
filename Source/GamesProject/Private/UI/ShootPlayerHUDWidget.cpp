// Shoot Game


#include "UI/ShootPlayerHUDWidget.h"
#include "Components/ShootHealtsComponent.h"	//���������� ��������� ��
#include "Weapon/ShootWeaponComponent.h"	//���������� ��������� ������
#include "ShootUtils.h"						//���������� ��������� ������� ��������� �����������


//������� ������� �������� � �� � ������ �� � ���������
float UShootPlayerHUDWidget::GetHealthPercent() const
{
	/*����������� � ������� GetHealthComponent//��������� �� ������ ������
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - ���������� ��������� �� pawn

	//�������� ���� ��� ��� ���������
	if (!Player) return 0.0f;

	//�������� ��������� �� ��� ���������
	const auto Component = Player->GetComponentByClass(UShootHealtsComponent::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)

	//�������� ��������� �� ��������� � ���������� ��������
	const auto HealthComponent = Cast <UShootHealtsComponent>(Component);	//UShootHealtsComponent - ��� � �������� �������. cast ��������� � �� ����*/

	//�������� ��������� �� ��������� � ���������� �������� � ������� ���������� ����� ShootUtils
	//const auto HealthComponent = GetHealthComponent();
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent< UShootHealtsComponent >(GetOwningPlayerPawn());

	//�������� �� ����
	if (!HealthComponent) return 0.0f;

	//���������� �� � ���������
	return HealthComponent->GetHealthPercent();

}

//�������� ������ � ���������� ������ � �������� ��� �������
//������� ������� ��� �������� ���������� ������, ��� �� �� ������ � ���������
//������� ������� ������� ���������� ������ ������ � ������� �������� ������. ��������� �������� �� ������ � �������� ��������� �������
bool UShootPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{

	/*��������� � �������//��������� �� ������ ������
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - ���������� ��������� �� pawn

	//�������� ���� ��� ��� ���������
	if (!Player) return false;

	//�������� ��������� �� ��� ���������
	const auto Component = Player->GetComponentByClass(UShootWeaponComponent::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)

	//�������� ��������� �� ��������� � ���������� ��������
	const auto WeaponComponent = Cast <UShootWeaponComponent>(Component);	//UShootHealtsComponent - ��� � �������� �������. cast ��������� � �� ����
	*/

	//������� ���������� ����� ShootUtils
	//const auto WeaponComponent = GetWeaponComponent();
	const auto WeaponComponent = ShootUtils::GetShootPlayerComponent< UShootWeaponComponent >(GetOwningPlayerPawn());

	//�������� �� ����
	if (!WeaponComponent) return false;
	
	//���������� �� � ���������
	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

//�������� ������ � �������� ��������
bool UShootPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{

	/*��������� � �������//��������� �� ������ ������
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - ���������� ��������� �� pawn

	//�������� ���� ��� ��� ���������
	if (!Player) return false;

	//�������� ��������� �� ��� ���������
	const auto Component = Player->GetComponentByClass(UShootWeaponComponent::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)

	//�������� ��������� �� ��������� � ���������� ��������
	const auto WeaponComponent = Cast <UShootWeaponComponent>(Component);	//UShootHealtsComponent - ��� � �������� �������. cast ��������� � �� ����
	*/

	//������� ���������� ����� ShootUtils
	//const auto WeaponComponent = GetWeaponComponent();
	const auto WeaponComponent = ShootUtils::GetShootPlayerComponent< UShootWeaponComponent >(GetOwningPlayerPawn());

	//�������� �� ����
	if (!WeaponComponent) return false;
	
	//���������� �� � ���������
	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

//��� �������� ��� ���, ���� ��� ������
bool UShootPlayerHUDWidget::IsPlayerAlive() const
{
	//�������� ��������� �� ��������� � ���������� �������� � ������� ���������� ����� ShootUtils
	//const auto HealthComponent = GetHealthComponent();
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent< UShootHealtsComponent >(GetOwningPlayerPawn());

	return HealthComponent && !HealthComponent->IsDead();	//����� ������ ���������� ������������� Spectate Pawn, ������� ��������� �� ��������� �� ����� � ����� ������� ��� �� ����� ��������. ������� ��� ��� ��� ����� ���������� �� �������� ��������� �� ��������� �������� 
}

//���������� true ����� �� � ������ �����������
bool UShootPlayerHUDWidget::IsPlayerSpectating() const
{
	//��������� �������� ����������� ������
	const auto Controller = GetOwningPlayer();	//GetOwningPlayer - ������� ��������� �������� ����������� ������ � user �������

	return Controller && Controller->GetStateName() == NAME_Spectating; //������� � ������ ����������� ���� ��������� �� ��� ��������� �� ������� � ����� ������� GetStateName() == NAME_Spectating
}

/*���������� � ��������� ������� ShootUtils
//������� ������� ���������� ��������� �� ��������� ������. ����������
UShootWeaponComponent* UShootPlayerHUDWidget::GetWeaponComponent() const
{
	//��������� �� ������ ������
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - ���������� ��������� �� pawn

	//�������� ���� ��� ��� ���������
	if (!Player) return nullptr;

	//�������� ��������� �� ��� ���������
	const auto Component = Player->GetComponentByClass(UShootWeaponComponent::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)

	//�������� ��������� �� ��������� � ���������� ��������
	const auto WeaponComponent = Cast <UShootWeaponComponent>(Component);	//UShootHealtsComponent - ��� � �������� �������. cast ��������� � �� ����

	return WeaponComponent;
}

//������� ������� ���������� ��������� �� ��������� ��������
UShootHealtsComponent* UShootPlayerHUDWidget::GetHealthComponent() const
{
	//��������� �� ������ ������
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - ���������� ��������� �� pawn

	//�������� ���� ��� ��� ���������
	if (!Player) return nullptr;

	//�������� ��������� �� ��� ���������
	const auto Component = Player->GetComponentByClass(UShootHealtsComponent::StaticClass());	//GetComponentByClass(������ subclass ���������� �������� ����� �����)

	//�������� ��������� �� ��������� � ���������� ��������
	const auto HealthComponent = Cast <UShootHealtsComponent>(Component);	//UShootHealtsComponent - ��� � �������� �������. cast ��������� � �� ����

	return HealthComponent;
}
*/

//������������� �� ������� ��������� ��������
bool UShootPlayerHUDWidget::Initialize()
{
	//�������� ��������� �� ��������� � ���������� �������� � ������� ���������� ����� ShootUtils
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent< UShootHealtsComponent >(GetOwningPlayerPawn());

	//�������� �� ����, ���� �� ���� �� ������������� �� �������
	if (HealthComponent)
	{
		HealthComponent->OnHeatlsChanged.AddUObject(this, &UShootPlayerHUDWidget::OnHeatlsChanged);	//� ���������� ������� ������� ��������� ��, ����������� ��������� � ����.�.�. �� ����� ��� � �++ �� ������� ���������� AddObject
	}

	return Super::Initialize();
}

void UShootPlayerHUDWidget::OnHeatlsChanged(float Health, float HealthDelta)
{
	//�������� ���� ��� ������� ����, �� ������ ����� �������� �������
	if (HealthDelta < 0.0f)
	{
		//���������� ��������� � ������������� ������ �������
		OnTakeDamage();
	}
	
}