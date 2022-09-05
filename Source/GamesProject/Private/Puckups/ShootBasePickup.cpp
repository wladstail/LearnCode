
#include "Puckups/ShootBasePickup.h"
#include "Components/SphereComponent.h"

//���� ������
DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);


AShootBasePickup::AShootBasePickup()
{
 	
	PrimaryActorTick.bCanEverTick = true;

    //������� ����� � ������� � �������
    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);                                         //������ ��������
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);              //����������� ������������ �� �����, ������ ���������� � ������������ ��������
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);   //������������ �������� �� ������ ������� ������� . �������� ������� ���������������
    SetRootComponent(CollisionComponent);

}


void AShootBasePickup::BeginPlay()
{
	Super::BeginPlay();

    //�������� ��� ������ ��������
    check(CollisionComponent);

    //�������� Pickup
    GenerateRotationYaw();
}

void AShootBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //�������� ���� ��������
    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f)); //���� �, �, z
}

//�������������� ������� ���������������
void AShootBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    //��� �� �� �������� ������ ������������ ������� NotifyActorBeginOverlap �����
    Super::NotifyActorBeginOverlap(OtherActor);

    //��������� ���������� ��������� �� Pawn.
    const auto Pawn = Cast<APawn>(OtherActor);//������ ���� ������, ������� ������ � �������� ��������� ������� � ������ Pawn

    //���� ������� ����� Pickup, �������� PickupWasTaken - ������ ���������
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();   //������ ���������
    }
    //UE_LOG(LogBasePickup, Display, TEXT("Pickup was taken"));
    
    //����������� pickup. �� ������� ������ ��������, ��� �� ����� ������ ���������
    //Destroy();
    //PickupWasTaken();
}

//���������� ����� ����� Pickup. ������ ��������� � ����������� ������
void AShootBasePickup::PickupWasTaken()
{
    //��������� ��� ��������, ��� �� �� ���������� BeginOverlap
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore); //����������� �������������� �������� � ���������� �����

    //�������� 
    if (GetRootComponent())
    {
        //�������� ��������� �� �������� ��������� ������ � ������ ��� ���������
        GetRootComponent()->SetVisibility(false, true); //SetVisibility (1 �������� ��������� �������, 2 visibilty ��������� ��� ���� �������� �����������). �.�. ���������� ��������� ��� ���������� ������
    }

    //��������� ������ ��������. ����� ���������. ������ ��������� ������������� ���, �.�. ����������� ����� ���� ��� � ��������� ��� �� �����
    FTimerHandle RespawnTimerHandle;

    //��� ������������ ���������� Respawn
    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AShootBasePickup::Respawn, RespawnTime);

}

//����������� �� ������������ ������� ��������
//���������� �������� � ������ ����� �������
void AShootBasePickup::Respawn()
{
    //�������������, ���� ����� ������ ��� ������ ��� ������ Pickup
    GenerateRotationYaw();

    //�������� ��� ��������, ��� �� ���������� BeginOverlap
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap); // �������������� �������� � ���������� �����

    //�������� 
    if (GetRootComponent())
    {
        //�������� ��������� �� �������� ��������� ������ � ������ ��� �������
        GetRootComponent()->SetVisibility(true, true); //SetVisibility (1 �������� ��������� �������, 2 visibilty ��������� ��� ���� �������� �����������)
    }
}

//� ������� ������ Pickup ������ ����, ������ ��� �� ������ ��� ����� ������ ����������
bool AShootBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

//��������� ���� ��������
void AShootBasePickup::GenerateRotationYaw()
{
    //����������� �������� 1 ��� -1 
    const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;    //RandBool ���������� �������� ������ ��� ����, ���� ������ ��������� 1, ����� -1. 

    //���� ��������
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;//��������� ��������� ����� �� �������(min, max). �� ������ ������� ��� ���� 0, ��� �� ��������� ���� 0
}