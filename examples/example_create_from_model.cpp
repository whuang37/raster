//example_create_from_model.cpp

#include <blink/raster/io.h>
#include <blink/raster/plot_raster.h>

namespace br = blink::raster;

int main()
{
  auto model = br::create<int>("test.tif", 3, 4);

  auto output = br::create_from_model<float>("test_float.tif", model);
  float i = 0.7f;
  for (auto&& v : output) {
    i = i * (1 - i);
    v = i;
  }
  plot_raster(output);
  return 0;
}