// Shoot Game


#include "UI/ShootPlayerHUDWidget.h"
#include "Components/ShootHealtsComponent.h"	//подключили компонент хп
#include "Weapon/ShootWeaponComponent.h"	//подключили компонент оружия
#include "ShootUtils.h"						//подключили шаблонную функцию получения компонентов


//функция которая доступна в БП и выдает хп в процентах
float UShootPlayerHUDWidget::GetHealthPercent() const
{
	/*переместили в функцию GetHealthComponent//указатель на нашего игрока
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - возвращает указатель на pawn

	//проверка если его нет указателя
	if (!Player) return 0.0f;

	//получаем указатель на наш компонент
	const auto Component = Player->GetComponentByClass(UShootHealtsComponent::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)

	//приводим указатель на компонент к компоненту здоровья
	const auto HealthComponent = Cast <UShootHealtsComponent>(Component);	//UShootHealtsComponent - тип к которому привели. cast проверяет и на нуль*/

	//приводим указатель на компонент к компоненту здоровья и сделали рефакторин через ShootUtils
	//const auto HealthComponent = GetHealthComponent();
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent< UShootHealtsComponent >(GetOwningPlayerPawn());

	//проверка на нуль
	if (!HealthComponent) return 0.0f;

	//возвращает хп в процентах
	return HealthComponent->GetHealthPercent();

}

//получаем доступ к компоненту оружия и вызываем его функцию
//функция обертка над функцией компонента оружия, что бы не делать в блупринте
//функция обертка которая возвращает иконки оружия и прицела текущего оружия. Структуру передаем по ссылке в качестве параметра функции
bool UShootPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{

	/*перенесли в функцию//указатель на нашего игрока
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - возвращает указатель на pawn

	//проверка если его нет указателя
	if (!Player) return false;

	//получаем указатель на наш компонент
	const auto Component = Player->GetComponentByClass(UShootWeaponComponent::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)

	//приводим указатель на компонент к компоненту здоровья
	const auto WeaponComponent = Cast <UShootWeaponComponent>(Component);	//UShootHealtsComponent - тип к которому привели. cast проверяет и на нуль
	*/

	//сделали рефакторин через ShootUtils
	//const auto WeaponComponent = GetWeaponComponent();
	const auto WeaponComponent = ShootUtils::GetShootPlayerComponent< UShootWeaponComponent >(GetOwningPlayerPawn());

	//проверка на нуль
	if (!WeaponComponent) return false;
	
	//возвращает хп в процентах
	return WeaponComponent->GetCurrentWeaponUIData(UIData);
}

//получаем доступ к текущему арсеналу
bool UShootPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{

	/*перенесли в функцию//указатель на нашего игрока
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - возвращает указатель на pawn

	//проверка если его нет указателя
	if (!Player) return false;

	//получаем указатель на наш компонент
	const auto Component = Player->GetComponentByClass(UShootWeaponComponent::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)

	//приводим указатель на компонент к компоненту здоровья
	const auto WeaponComponent = Cast <UShootWeaponComponent>(Component);	//UShootHealtsComponent - тип к которому привели. cast проверяет и на нуль
	*/

	//сделали рефакторин через ShootUtils
	//const auto WeaponComponent = GetWeaponComponent();
	const auto WeaponComponent = ShootUtils::GetShootPlayerComponent< UShootWeaponComponent >(GetOwningPlayerPawn());

	//проверка на нуль
	if (!WeaponComponent) return false;
	
	//возвращает хп в процентах
	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}

//жив персонаж или нет, если жив истина
bool UShootPlayerHUDWidget::IsPlayerAlive() const
{
	//приводим указатель на компонент к компоненту здоровья и сделали рефакторин через ShootUtils
	//const auto HealthComponent = GetHealthComponent();
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent< UShootHealtsComponent >(GetOwningPlayerPawn());

	return HealthComponent && !HealthComponent->IsDead();	//после смерти контроллер захватывается Spectate Pawn, поэтому указатель на персонаже не будет и после никогда жив не будет персонаж. Поэтому жив или нет будет опредяться по нулевому указателю на компонент здоровья 
}

//возвращает true когда мы в режиме наблюдателя
bool UShootPlayerHUDWidget::IsPlayerSpectating() const
{
	//получения текущего контроллера игрока
	const auto Controller = GetOwningPlayer();	//GetOwningPlayer - функция получения текущего контроллера игрока в user виджете

	return Controller && Controller->GetStateName() == NAME_Spectating; //находим в режиме наблюдателя пока указатель на наш котроллер не нулевой и когда функция GetStateName() == NAME_Spectating
}

/*Реализация в шаблонной функции ShootUtils
//функция которая возвращает указатель на компонент оружия. Косметичка
UShootWeaponComponent* UShootPlayerHUDWidget::GetWeaponComponent() const
{
	//указатель на нашего игрока
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - возвращает указатель на pawn

	//проверка если его нет указателя
	if (!Player) return nullptr;

	//получаем указатель на наш компонент
	const auto Component = Player->GetComponentByClass(UShootWeaponComponent::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)

	//приводим указатель на компонент к компоненту здоровья
	const auto WeaponComponent = Cast <UShootWeaponComponent>(Component);	//UShootHealtsComponent - тип к которому привели. cast проверяет и на нуль

	return WeaponComponent;
}

//функция которая возвращает указатель на компонент здоровья
UShootHealtsComponent* UShootPlayerHUDWidget::GetHealthComponent() const
{
	//указатель на нашего игрока
	const auto Player = GetOwningPlayerPawn();	//GetOwningPlayerPawn - возвращает указатель на pawn

	//проверка если его нет указателя
	if (!Player) return nullptr;

	//получаем указатель на наш компонент
	const auto Component = Player->GetComponentByClass(UShootHealtsComponent::StaticClass());	//GetComponentByClass(внутри subclass КОМПОНЕНТа которого хотим найти)

	//приводим указатель на компонент к компоненту здоровья
	const auto HealthComponent = Cast <UShootHealtsComponent>(Component);	//UShootHealtsComponent - тип к которому привели. cast проверяет и на нуль

	return HealthComponent;
}
*/

//подписываемся на делегат изменения здоровья
bool UShootPlayerHUDWidget::Initialize()
{
	//приводим указатель на компонент к компоненту здоровья и сделали рефакторин через ShootUtils
	const auto HealthComponent = ShootUtils::GetShootPlayerComponent< UShootHealtsComponent >(GetOwningPlayerPawn());

	//проверка на нуль, если не нуль то подписываемся на делегат
	if (HealthComponent)
	{
		HealthComponent->OnHeatlsChanged.AddUObject(this, &UShootPlayerHUDWidget::OnHeatlsChanged);	//в компоненте находим делегат изменения хп, подписываем персонажа к нему.т.к. он будет ток в С++ то функцию используем AddObject
	}

	return Super::Initialize();
}

void UShootPlayerHUDWidget::OnHeatlsChanged(float Health, float HealthDelta)
{
	//проверка если был нанесен урон, то только тогда вызываем событие
	if (HealthDelta < 0.0f)
	{
		//оповещения блупринта о возникновении нашего события
		OnTakeDamage();
	}
	
}