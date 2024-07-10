#pragma once

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
#define PRINT_ON_SCREEN(Duration, Color, String, NewerOnTop) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, Color, String, NewerOnTop);

#define DRAW_SPHERE(Location, Radius, NumberOfSegments, Color, LifeTime) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, Radius, NumberOfSegments, Color, false, LifeTime, 0.0f, 1.0f);
#define DRAW_LINE(StartLocation, EndLocation, Color, LifeTime) if (GetWorld()) DrawDebugLine(GetWorld(), StartLocation, EndLocation, Color, false, LifeTime, 0, 1.0f);
#define DRAW_POINT(Location, Size, Color, LifeTime) if(GetWorld()) DrawDebugPoint(GetWorld(), Location, Size, Color, false, LifeTime, 0);
#define DRAW_STRING(Location, String, Color, Duration) if (GetWorld()) DrawDebugString(GetWorld(), Location, String, nullptr, Color, Duration);

#else
#define PRINT_ON_SCREEN(Duration, String, NewerOnTop)

#define DRAW_SPHERE(Location, Radius, NumberOfSegments, Color, LifeTime)													
#define DRAW_LINE(StartLocation, EndLocation, Color, LifeTime)
#define DRAW_POINT(Location, Size, Color, LifeTime)
#define DRAW_STRING(Location, String, Color, Duration)
#endif