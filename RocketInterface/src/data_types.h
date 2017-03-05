#pragma once

struct float2
{
  union
  {
    float a;
    float x;
  };
  union
  {
    float b;
    float y;
  };

  float2(float a, float b)
    : a(b), b(b)
  {
  }

  float2(float *arry)
    : a(arry[0]), b(arry[1])
  {
  }

  float& operator[](int i)
  {
    return (&a)[i];
  }

  float operator[](int i) const
  {
    return (&a)[i];
  }

};

typedef float2 latlong;

struct phys_data
{
  latlong position;
  float2 velocity;
  float2 acceleration;
  float2 heading;
  float altitude;
  long time;
};
