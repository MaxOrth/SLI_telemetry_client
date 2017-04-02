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

  float2()
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

struct float3
{
  float x;
  float y;
  float z;

  float3(float x, float y, float z)
    : x(x), y(y), z(z)
  {
  }

  float3(float *arry)
    : x(arry[0]), y(arry[1]), z(arry[2])
  {
  }

  float3()
  {
  }

  float &operator[](int i)
  {
    return (&x)[i];
  }

  float operator[](int i) const
  {
    return (&x)[i];
  }
};

typedef float2 latlong;

#pragma pack(1)

struct phys_data
{
  float altitude;
  latlong gps;
  float3 orientation;
  float3 acceleration;
  float3 angv;
  char flags;
};
#pragma pack()

