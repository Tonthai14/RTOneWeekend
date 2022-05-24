# RTOneWeekend
This code produces a PPM format image using a basic raytracer. 
I followed the tutorial here: https://raytracing.github.io/books/RayTracingInOneWeekend.html
Spheres of materials/surfaces of lambertian, metal, or dielectric (glass) can be placed within the scene and will reflect and/or refract light rays based on where they are placed relative to the camera.
Additionally, sharper images can be produced when the samples per pixel is increased, however, it will take a considerably longer time to render.

Final Image:
![image](https://user-images.githubusercontent.com/46981841/170136678-4aabde0f-8d37-449b-86fe-0e23de39a12c.png)
This uses only 10 samples per pixel, so the image is noticeably fuzzy/blurry. 
