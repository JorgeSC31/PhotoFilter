# PhotoFilter
PhotoFilter es un proyecto para la materia de Introducción a la computación de la Lic. en Computación Matemática en DEMAT.

PhotoFilter es un programa para editar imagenes de distintos formatos (jpg, jpeg, bmp, png, ect). Esté programa se creo usando la libreria de SDL2.

El editor de imagenes te permite agregar filtro de B/N, invrsión de color o sepia, manipular los valores rgb de la imagen. También puedes voltear la imagen horizontal y verticalmente. Todas estas funciones se aplican a la imagen en tiempo real, además de que se pueden activar múltiples opciones al mismo tiempo.
Es perfecto para ver distintos objetos en una imagen cambiando la configuración de color para hacer mas visible ciertos objetos.



# Compilar y Ejecutar
Antes de compilar ejecuta el siguiente comando:
xargs -a requirements.txt sudo apt-get install


g++ -o PhotoFilter main.cpp LTexture.cpp LButton.cpp common.cpp -lSDL2 -lSDL2_image
./PhotoFilter Examples/img2.jpg

Al momento de ejecutar debes pasar el argumento con la ruta de la imagen.
