


#include "Player/ShootBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"					//������ ���
#include "GameFramework/SpringArmComponent.h"			//�������� ������ ���
#include "Components/Shoot_CharacterMovementComponent.h" //��������� ���� ��������
#include "Components/ShootHealtsComponent.h"			//��������� ��
#include "Components/TextRenderComponent.h"
#include "Weapon/ShootWeaponComponent.h"		//������ ���������
#include "Components/CapsuleComponent.h"	//��������
#include "GameFramework/Controller.h"		//���������� ������


//#include "Weapon/ShootBaseWeapon.h"			//������ ��������� � ��������� ������

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);


//������� ���������� ���������
AShootBaseCharacter::AShootBaseCharacter(const FObjectInitializer& ObjInit):Super(ObjInit.SetDefaultSubobjectClass<UShoot_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) //SetDefaultSubobjectClass<����� ������� ������� ����� ��������>
{
 	
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject <USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());	  					    //������������ � ���������
	SpringArmComponent->bUsePawnControlRotation = true;								//��� �������� ����� ����������

		//��� ��������� ��������� ������ ��
	SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);				//����� ���������� �� ����������


	CameraComponent = CreateDefaultSubobject <UCameraComponent>("CameraComponent"); //�� ��������� ������� ���������
	CameraComponent->SetupAttachment(SpringArmComponent);							 //GetRootComponent ���������� ��������� �� ������ ��������� ������
	//� ������� ������� ��������� � Use Pawn Control Rotation ��� �� ������� ��� ������ � ���������



	HealtsComponent = CreateDefaultSubobject <UShootHealtsComponent>("HealtsComponent");	//��������� ��, �� ���������� ������������ �� �����������

	HealtsTextComponent = CreateDefaultSubobject <UTextRenderComponent>("HealtsTextComponent");
	HealtsTextComponent->SetupAttachment(GetRootComponent());

	//��� ����� ������ ��� ������ ��������
	HealtsTextComponent->SetOwnerNoSee(true);	//��� ����� ��� ����������, �� ������ �� �����


	WeaponComponent = CreateDefaultSubobject <UShootWeaponComponent>("WeaponComponent");		//��������� ������

}

//�� ��������� � �������������� �� ����
void AShootBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//���������� ������ � ����� ������ 
	check(HealtsComponent);		//���� ������� ��������� ����������� ������
	check(HealtsTextComponent);
	check(GetCharacterMovement());
	check(GetMesh());	//�������� ��� ���� ��� � ���������

	//������������ ������� ����
	OnHealtsChanged(HealtsComponent->GetHealts(), 0.0f); //�������� � ������ ���� 100 �� � ������� ���, ��� ����� ��� � ������ ����� 0 ��. ������ �������� �������� ��-�� ����� �������� ������ � ��������

	//������� ������������ ��� ������ ���������
	HealtsComponent->OnDeath.AddUObject(this, &AShootBaseCharacter::OnDeath);	//� ���������� ������� ������� ������ ����������� ��������� � ����.�.�. �� ����� ��� � �++ �� ������� ���������� AddObject
	
	//������� �������� ����� ��� ��������� ��������
	HealtsComponent->OnHeatlsChanged.AddUObject(this, &AShootBaseCharacter::OnHealtsChanged); //������ ����� �� ������ ����� ���������� ���, � �� ������ ���
	//OnTakeAnyDamage.AddDynamic(this, &AShootBaseCharacter::OnTakeAnyDamageHandle);	//��������� ������� ������

	//��� ��������� ����� ������� � ������
	LandedDelegate.AddDynamic(this, &AShootBaseCharacter::OnGroundLanded);//1 �� 2 ������� � ������� ����� ������������ �������. ��������� �������� � Landed


	
	//SpawnWeapons();		//����� ������ �������� � ��������� 
}

//����� �� ��������� ����������� ��� ������ �������� ��� ��������� ��
void AShootBaseCharacter::OnHealtsChanged(float Healts, float HealthDelta)
{
	HealtsTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Healts)));	//����� ������ ��
}

void AShootBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const auto Healts = HealtsComponent->GetHealts();	//������ ��� �������� ���� ��

	// HealtsTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Healts)));	//����� ������ ��




	//TakeDamage(0.1f, FDamageEvent(), Controller, this);	//����, ����� ����� �� ����, ��������� �� ���������� ���������, ��������� ��� ������� ����
	

}

//�������� ���������, � ��� �� ��������
void AShootBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShootBaseCharacter::MoveForward);				//��������� �������
	PlayerInputComponent->BindAxis("MoveRight", this, &AShootBaseCharacter::RightForward);

	PlayerInputComponent->BindAxis("LookUp", this, &AShootBaseCharacter::AddControllerPitchInput);						//��������� ���� ������ this, &AShootBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("TurnAround", this, &AShootBaseCharacter::AddControllerYawInput);				//������  this, & AShootBaseCharacter::TurnAround);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AShootBaseCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AShootBaseCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AShootBaseCharacter::StopRun);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &UShootWeaponComponent::StartFire);	//�������� ��� �� ��������� � ���������
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &UShootWeaponComponent::StopFire);	//�������� ��� �� ��������� � ���������

	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, WeaponComponent, &UShootWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &UShootWeaponComponent::Reload);
}

//�������� �������� ���������
void AShootBaseCharacter::MoveForward(float Amount)
{
	RunCharacter = Amount > 0.0f;							//�������� ����� �� ��������
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);		//GetActorForwardVector,GetActorRightVector,GetActorUpVector ��� ��� x,y,z

}	   
void AShootBaseCharacter::RightForward(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);

}
void AShootBaseCharacter::Run()
{
	RunKeyActiv = true;


}
void AShootBaseCharacter::StopRun()
{
	RunKeyActiv = false;
}
bool AShootBaseCharacter::isRuning() const
{
	return RunKeyActiv && RunCharacter && !GetVelocity().IsZero();	//���������� ��������, ���� ��� True �� True, ����� False
}

//�������� ��������� ��� ���������
float AShootBaseCharacter::GetMovementDirection() const	//��� �������� ��� ��������� ���������
{
	if (GetVelocity().IsZero()) return 0.0f; //���� ��� ���������� True, �� ���������� ����
	const auto VelocityNormal = GetVelocity().GetSafeNormal();	//auto ������������� ����������� ����. GetSafeNormal - ���������� �������
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));		//���� ����� ����� ������� �������� � �������� ��������
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);			//��������� ������������ ��������. ������������� ������	
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);					//������� ������ ��� ��� �������� ����� ������ ���� ����� ���� ��� � ������
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);		// FMath::RadiansToDegrees(AngleBetween) * FMath::Sign(CrossProduct.Z);
}
/*
void AShootBaseCharacter::LookUp(float Amount)
{

	AddControllerPitchInput(Amount);			//�.�. ������ �� ���������� � ��������� ������ ���� �������� �����, ������ �� ������

}
void AShootBaseCharacter::TurnAround(float Amount)
{

	AddControllerYawInput(Amount);					//�������� �����������

}
*/

//������ ��������� � � �������� � ������ �������� ����� ������
void AShootBaseCharacter::OnDeath() 
{

	UE_LOG(BaseCharacterLog, Display, TEXT("Player %sis Dead"), *GetName());

	//������������ �������� �� ����( � ���������� ������ �������� )
	//�������� �� RagDool PlayAnimMontage(DeathAnimMontage);	//�������� ������� ������� ������

	//���������� ������������
	GetCharacterMovement()->DisableMovement();//��������� ��� ��������

	//����������� ��������� � ��������
	SetLifeSpan(LifeSpan); // ����������� ��������� ����� 5 ������
						   
	//��������� ���������� ����� ������
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);	//�������� ����� ������ � �������
	}

	//��� �� �� ����� ��������� ����� ��������
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);	//�������� �������, ������� � ������������

	//���� ���� �������� ����� ������( ������ �������� ���� ) 
	WeaponComponent->StopFire();

	//�������� ��� �������� ���� ��������� ������
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);//�������� ��������� �� �������� ��� ���������->������� �������� ����������

	//�������� ��������� ������ �� ���
	GetMesh()->SetSimulatePhysics(true); //�������� ��������� �� �������� ��� ��������->��� ��������� ������
}

//���� ��������� ��� ������
void AShootBaseCharacter::OnGroundLanded(const FHitResult& Hit)	//���� � ������
{
	//�������� �������
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;//��������� FallVelosity ������ ���������� ��������.����� ���� �������� � ����� GetVelocity().Z 
	UE_LOG(BaseCharacterLog, Display, TEXT("on Landed: %f"), FallVelocityZ);	//������� �������� �������� �����������

	//������� �� ������� ���� ���� ������ ��������� ���������
	if (FallVelocityZ < LandedDamageVelocity.X) return; //���� �������� max �������� ������ ��������� �� �������. ��� ������� ������ �������� �������������� ������� *(-1)

	//��������� �����
	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);//1 ������� ������� 2 �������� ������� 3 �������� � ������� �����. ������� ����� �� ������� � ���������� ����������� �� ������� LandedDamageVelocity
	UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);	//�������� ����
};

/*��������� � ������
void AShootBaseCharacter::SpawnWeapons()	//����� ������ � ������������� ���� � ������ ������(����� ������ ��������� � ���� ����)
{
	if (!GetWorld()) return;	//�������� ��� ��������� �� ��� �� �������

	const auto Weapon = GetWorld()->SpawnActor<AShootBaseWeapon>(WeaponClass);	//����� ������ ������. SpawnActor - ���������� ��������� �� ��������� ������. ������� ����� ��������� ��������� � ���������� Weapon
	if (Weapon)	//�������� ��� ������ ��������
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);//1 ������������� ������������� ��������������� ���������� � ���� �� ������ ������� 2 ��������� ��� ���
		Weapon->AttachToComponent(GetMesh(), AttachmentRules, "WeaponSocket");	//������������ ��� � ������ ���������.1 ��������� �� ��������� � �������� ����� �������������� 3 �������� ������ GetMesh - ���������� ��������� �� Skeleton mesh
	}
}
*/