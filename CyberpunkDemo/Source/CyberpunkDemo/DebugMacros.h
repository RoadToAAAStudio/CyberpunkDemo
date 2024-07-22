#pragma once

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#define PRINT_ON_SCREEN(Duration, Color, String, NewerOnTop) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, String, NewerOnTop);

#define DRAW_SPHERE(OwnerActor, Location, Radius, NumberOfSegments, Color, LifeTime) if (OwnerActor->GetWorld()) DrawDebugSphere(OwnerActor->GetWorld(), Location, Radius, NumberOfSegments, Color, false, LifeTime, 0.0f, 1.0f);
#define DRAW_LINE(OwnerActor, StartLocation, EndLocation, Color, LifeTime) if (OwnerActor->GetWorld()) DrawDebugLine(OwnerActor->GetWorld(), StartLocation, EndLocation, Color, false, LifeTime, 0, 1.0f);
#define DRAW_POINT(OwnerActor, Location, Size, Color, LifeTime) if(OwnerActor->GetWorld()) DrawDebugPoint(OwnerActor->GetWorld(), Location, Size, Color, false, LifeTime, 0);
#define DRAW_STRING(OwnerActor, Location, String, Color, Duration) if (OwnerActor->GetWorld()) DrawDebugString(OwnerActor->GetWorld(), Location, String, nullptr, Color, Duration);

#else
#define PRINT_ON_SCREEN(Duration, String, NewerOnTop)

#define DRAW_SPHERE(OwnerActor, Location, Radius, NumberOfSegments, Color, LifeTime)													
#define DRAW_LINE(OwnerActor, StartLocation, EndLocation, Color, LifeTime)
#define DRAW_POINT(OwnerActor, Location, Size, Color, LifeTime)
#define DRAW_STRING(OwnerActor, Location, String, Color, Duration)
#endif