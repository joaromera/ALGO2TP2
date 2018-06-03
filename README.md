# Base De Datos

## Algoritmos y Estructuras de Datos 2 - Trabajo Práctico 2

El enunciado se encuentra en la carpeta *docs*
Todo el código generado para el trabajo está en *src*
Puede importarse el repositorio en CLion y correr los tests.

El trabajo consistió en implementar una base de datos con una funcionalidad mínima de crear tablas, ingresar datos y poder hacer joins entre distintas tablas.

# Para empezar 

Es necesario bajar el código del este repositorio. Esto puede hacerse
utilizando directamente `git` o alternativamente bajando el proyecto como un
`zip`. Se recomienda la primera opción que se explica a continuación.

1. `git clone https://git.exactas.uba.ar/mmiguel/aed2-2c2017-tp2.git`
2. (Opcional) Desde CLion, ir a File -> Import Project, "Open Project".
 Una vez que termina la carga del proyecto, para correr los tests, 
 elegir "correrTests" desde la esquina superior derecha y 
 hacer click en el botón Play.
 
# Cómo correr los tests desde la línea de comandos (si no usan CLion)

1. mkdir build && cd build
2. cmake ..
3. make
4. ./correrTests

Para volver a compilar, simplemente correr "make" de nuevo.

En caso de que no funcione correctamente la compilación, pueden borrar 
el directorio "build" y volver a realizar los 4 pasos.
