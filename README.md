
# PPM Image Steganography Application using C

Using this application, you can encode a text message inside a PPM image. Also, you can decode a message encrypted inside a PPM image by comparing it with the original PPM image.
## Getting Started

* Programming language: C

* Development environment: Visual Studio Code

#### Step 1

Copy the source folder (Steganography) to the desired location (ex: Desktop).

#### Step 2

Open command prompt and open the folder.

`cd Desktop/Steganography`

#### Step 3

Compile the program. Since the Makefile has been created, you can type the relevant command to execute the Makefile.

`mingw32-make`

![Picture1](https://user-images.githubusercontent.com/69090976/166671972-a2185d9f-51e1-4bec-b1f2-c86b3130685e.png)

Important: If you’re using Windows, you need to have a C Compiler installed on your PC (eg:MinGW). 

### Run the program

#### Case 1: Test

`steg t ape.ppm`

![Picture2](https://user-images.githubusercontent.com/69090976/166672134-8d7fc18f-2619-4bef-920a-99fb7886cef9.png)

Here the ape.ppm is the PPM Image in the Steganography folder. This file can be replaced with any file you like.

#### Case 2: Encode

`steg e ape.ppm`

![Picture3](https://user-images.githubusercontent.com/69090976/166672166-e896b9d8-d64f-4b5a-be7a-f6d94460e8a7.png)

#### Case 3: Decode

`steg d ape.ppm modified.ppm`

![Picture4](https://user-images.githubusercontent.com/69090976/166672196-de275c5e-8cba-4268-8099-252d30dd203f.png)

You will be able to see the new folders (modified.ppm, steg.exe) created inside the source folder.

![Picture5](https://user-images.githubusercontent.com/69090976/166672222-b4b9744c-9dcb-41af-8151-4d01ece7e2e8.png)
